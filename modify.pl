#!/usr/bin/perl

use strict;
use warnings;
use File::Basename;
use Term::ANSIColor;
use Cwd 'abs_path';

sub dprint (@) {
	foreach (@_) {
		print color("green")."modify.pl:::".$_.";".color("reset")."\n";
	}
}

main(@ARGV);

sub main {
	foreach my $filename (@_) {
		dprint "main($filename)";
		my $contents = read_file($filename);
		my $folder = dirname(abs_path($filename));
		my $modified = modify($contents, $folder);
		write_file($filename, $modified);
	}
}

sub write_file {
	my $filename = shift;
	my $contents = shift;
	dprint "write_file($filename, \$contents)";

	open my $fh, '>', "$filename.modified.c";
	print $fh $contents;
	close $fh;
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

sub modify {
	my $code = shift;
	my $folder = shift;

	dprint "modify(\$code)";
	$code =~ s#\R\s*void main#\nint main#;
	$code =~ s#return;#return 0;#g;

	if($code =~ m#pszNewLinkName#) {
		system("touch $folder/file.dat");
		system("touch file.dat");
$code = qq#
const char *pszNewLinkName;
pszNewLinkName = "test.dat";
WCHAR *pszExistingFileName = L"file.dat";
$code#;
	}

	if($code !~ m#int main#) {
		$code = "
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>

int main () {\n$code\n\treturn 0;\n}\n";
	}

	return $code;
}
