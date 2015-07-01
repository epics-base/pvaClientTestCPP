# Perl module that starts the exampleDatabase,
# then runs a test program while the IOC is active.

use strict;
use warnings;

use IOC;

$ENV{HARNESS_ACTIVE} = 1 if scalar @ARGV && shift eq '-tap';

my $arch = $ENV{EPICS_HOST_ARCH};

my $iocTop = '../../database';
my $iocExe = "$iocTop/bin/$arch/exampleDatabase";
my $iocArg = "$iocTop/iocBoot/exampleDatabase/st.cmd";

sub runTest {
    my $test = shift;

    # Create IOC object
    my $ioc = IOC->new();
    # $ioc->debug(1);

    # Run it
    $ioc->start($iocExe, $iocArg);

    # Wait for command prompt after startup script finishes
    $ioc->cmd;

    # Run the test program
    my $ret = system "./$test";

    # Stop the IOC
    $ioc->kill;

    # Finish
    exit $ret;
}

1;
