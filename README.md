# riscof_install_example_DUT
Example DUT for riscof as well as install scripts for installing riscof on both Ubuntu and RHEL8

This assumes that python3.6 and pip3 or later is installed on your system.<br>

<hr>
Install under Ubuntu<br>

This will install riscof under your ~/.local/bin dir<br>
./install_scripts/install_riscof.sh<br>

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

Install sail dependencies, you will need sudo access for this step.<br>
./install_scripts/sudo_install_sail.sh<br>

Clone and compile Sail, clone sail plugin and riscv-arch-test<br>
./install_scripts/local_install_sail.sh<br>

<hr>

Install under RHEL8<br>

This will install riscof under your ~/.local/bin dir<br>
./install_scripts/install_riscof_rhel8.sh<br>

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

Install sail dependencies, you will need sudo access for this step.<br>
./install_scripts/sudo_install_sail_rhel8.sh<br>

Clone and compile Sail, clone sail plugin and riscv-arch-test. It will also downlaod the latest Z3 source and compile it.<br>
All installed binaries/artifacts will be under ~/.local/bin<br>
./install_scripts/local_install_sail_rhel8.sh<br>
