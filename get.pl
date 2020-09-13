#!/usr/bin/perl

use strict;
use warnings;
use Term::ANSIColor;
use HTML::Entities;
use Data::Dumper;

sub dprint (@) {
	foreach (@_) {
		print color("green")."get.pl:::".$_.";".color("reset")."\n";
	}
}

my $list = read_file("list.html");
my @headers = ();

while ($list =~ m#href="https://docs.microsoft.com/en-us/windows/win32/api/([^"]*)/?"#gism) {
	my $match = $1;
	$match =~ s#/$##g;
	push @headers, $match;
}

foreach my $header (sort { sort() <=> sort() } @headers) {
	parse_overview("https://docs.microsoft.com/en-us/windows/win32/api/$header");
}

sub parse_overview {
	my $url = shift;
	my $name = $url;
	$name =~ s#.*/##g;

	get($url, "overview");
	my $filename = "pages/overview/$name";

	my $content = read_file($filename);

	my @urls = ();

	while ($content =~ m#<td><a href="/en-us/windows/win32/api/([^"]+)" data-linktype="absolute-path">[^<]+</a></td>#gism) {
		push @urls, "https://docs.microsoft.com/en-us/windows/win32/api/$1";
	}
	
	foreach my $url (sort { rand() <=> rand() } @urls) {
		parse_and_run($url);
	}
}

sub parse_and_run {
	my $url = shift;
	get($url);
	my $filename = $url;

	$filename =~ s#.*/##g;
	my $function_name = $filename;
	$filename = "pages/$filename";

	my $library = $url;
	$library =~ s#.*/(.*?)/.*?$#$1#g;

	my $contents = read_file($filename);

	my $min_version = undef;
	my $example = undef;

	if($contents =~ m#<meta name="req.target-min-winverclnt" content="Windows[^"]*(2000|XP|Vista|10|8\.1)[^"]*" />#) {
		$min_version = $1;
	}

	if($contents =~ m#<h4 id="examples">Examples</h4>.*?<pre><code class="lang-cpp">(.*?)</code></pre>#gism) {
		$example = decode_entities($1);
	}

	if($example && $min_version && $library) {
		system("mkdir -p ./win/$min_version/$library/");
		my $path = "./win/$min_version/$library/$function_name.c";
		write_file($path, $example);
		system("make run TARGET=$path");

		if($?) {
			print color("red")."The file $path could not be built or it could not be executed by wine correctly.".color("reset")."\n";
			system("echo $min_version/$library/$function_name >> fails.txt");
		} else {
			print color("green")."The file $path could be built and wine correctly.".color("reset")."\n";
			system("echo $min_version/$library/$function_name >> success.txt");
		}
	} else {
		my $string = 'An error occured with URL $url;';
		$string .= "\n\tCould not find example" if !$example;
		$string .= "\n\tCould not find library" if !$library;
		$string .= "\n\tCould not find min_version" if !$min_version;
		warn color("red").$string.color("reset")."\n";
	}
}

sub get {
	my $url = shift;
	my $prefix = shift // '';

	warn $url;

	if($prefix) {
		system("wget -nc --directory-prefix=pages/$prefix $url")
	} else {
		system("wget -nc --directory-prefix=pages $url")
	}
}

sub read_file {
	my $filename = shift;
	dprint "read_file($filename)";
	my $contents = '';
	open my $fh, '<', $filename;
	while (<$fh>) {
		$contents .= $_;
	}
	close $fh;
	return $contents;
}

sub write_file {
	my $filename = shift;
	my $contents = shift;
	dprint "write_file($filename, \$contents)";

	open my $fh, '>', "$filename";
	print $fh $contents;
	close $fh;
}
