# riscof_install_example_DUT
Example DUT for riscof as well as install scripts for installing riscof on both Ubuntu and RHEL8

<hr>
Install under Ubuntu<br>
<hr>

You will have to have sudo priviliges for the first step:<br> 
This will install all the dependencis needed for sail/riscof<br>
sudo ./install_scripts/ubuntu/sudo_install_sail.sh<br>

This will install riscof/sail under ~/.local/bin dir<br>
./initall_ubuntu.sh<br>

Test riscof by running riscof --help, you should see something similar to:

Usage: riscof [OPTIONS] COMMAND [ARGS]...

    Options:
      --version                       Show the version and exit.
      -v, --verbose [info|error|debug]
                                      Set verbose level
      --help                          Show this message and exit.

    Commands:
      arch-test     Setup and maintenance for Architectural TestSuite.
      coverage      Run the tests on DUT and reference and compare signatures
      gendb         Generate Database for the Suite.
      run           Run the tests on DUT and reference and compare signatures
      setup         Initiate Setup for riscof.
      testlist      Generate the test list for the given DUT and suite.
      validateyaml  Validate the Input YAMLs using riscv-config.

<hr>
Install under RHEL8<br>
<hr>

You will have to have sudo priviliges for the first step:<br> 
This will install all the dependencis needed for sail/riscof<br>
./install_scripts/rhel/sudo_install_sail_rhel8.sh<br>

This will install riscof/sail under ~/.local/bin dir<br>
./initall_rhel.sh<br>

Test riscof by running riscof --help, you should see something similar to:

Usage: riscof [OPTIONS] COMMAND [ARGS]...

    Options:
      --version                       Show the version and exit.
      -v, --verbose [info|error|debug]
                                      Set verbose level
      --help                          Show this message and exit.

    Commands:
      arch-test     Setup and maintenance for Architectural TestSuite.
      coverage      Run the tests on DUT and reference and compare signatures
      gendb         Generate Database for the Suite.
      run           Run the tests on DUT and reference and compare signatures
      setup         Initiate Setup for riscof.
      testlist      Generate the test list for the given DUT and suite.
      validateyaml  Validate the Input YAMLs using riscv-config.
<hr>

All installed binaries/artifacts will be under for both rhel and ubuntu ~/.local/bin<br>
