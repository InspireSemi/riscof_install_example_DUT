import os
import re
import shutil
import subprocess
import shlex
import logging
import random
import string
from string import Template
import sys

from pathlib import PurePath

import riscof.utils as utils
import riscof.constants as constants
from riscof.pluginTemplate import pluginTemplate

logger = logging.getLogger()

class example_dut(pluginTemplate):
    __model__ = "example_dut"

    #DUT version
    __version__ = "1.0"

    def __init__(self, *args, **kwargs):
        sclass = super().__init__(*args, **kwargs)

        # Assume this reads the config.ini file? 
        config = kwargs.get('config')

        self.dut_exe = os.path.join(config['PATH'] if 'PATH' in config else "","exampel_dut")
        self.num_jobs = str(config['jobs'] if 'jobs' in config else 1)
        if config is None:
            print("Please enter input file paths in configuration.")
            raise SystemExit
        else:
            self.isa_spec = os.path.abspath(config['ispec'])
            self.platform_spec = os.path.abspath(config['pspec'])
            self.pluginpath=os.path.abspath(config['pluginpath'])

        return sclass

    def initialise(self, suite, work_dir, archtest_env):
        self.work_dir = work_dir

        self.gccopts = '-static -mcmodel=medany -g -fvisibility=hidden -nostdlib -nostartfiles -DXLEN={1}'
        self.compile_cmd = 'riscv64-unknown-elf-gcc -march={0} ' + self.gccopts + \
         ' -T ' + self.pluginpath + '/env/link.ld \
         -I ' + self.pluginpath + '/env/ -I ' + archtest_env

        self.binarytools = self.pluginpath + '/bin/'
        self.bin2hex = self.binarytools + 'freedom_bin2hex'
        

    def build(self, isa_yaml, platform_yaml):
        ispec = utils.load_yaml(isa_yaml)['hart0']
        self.xlen = ('64' if 64 in ispec['supported_xlen'] else '32')
        self.isa = 'rv' + self.xlen
        #TODO: The following assumes you are using the riscv-gcc toolchain. If
        #      not please change appropriately
        self.compile_cmd = self.compile_cmd+' -mabi='+('lp64 ' if 64 in ispec['supported_xlen'] else 'ilp32 ')
        if "I" in ispec["ISA"]:
            self.isa += 'i'
        if "M" in ispec["ISA"]:
            self.isa += 'm'
        if "C" in ispec["ISA"]:
            self.isa += 'c'

        # based on the validated isa and platform configure your simulator or
        # build your RTL here

    def runTests(self, testList, cfg_file=None):
        make = utils.makeUtil(makefilePath=os.path.join(self.work_dir, "Makefile." + self.name[:-1]))
        make.makeCommand = 'make -j' + self.num_jobs
        for file in testList:
            testentry = testList[file]
            test = testentry['test_path']
            test_dir = testentry['work_dir']

            # This should return the filename of the source file. add-01.S for example 
            source = PurePath(test).name
            # sourcefilename.elf (ie add-01.S.elf)
            elf = source + '.elf'

            self.readelfopts = '-a ' + elf + '> ' + elf + '.readelf '
            self.objdumpopts = '-x -S -D ' + elf + '> ' + elf + '.objdump '
            self.objcopyopts = '-S -R .comment -R .note.gnu.build-id -O binary ' + elf + ' ' + elf + '.bin'
            self.objcopy = 'riscv64-unknown-elf-objcopy ' + self.objcopyopts + ';'
            self.readelf = 'riscv64-unknown-elf-readelf ' + self.readelfopts + ';'
            self.objdump = 'riscv64-unknown-elf-objdump ' + self.objdumpopts + ';'
            
            execute = "@cd "+testentry['work_dir']+";"

            cmd = self.compile_cmd.format(testentry['isa'].lower(), self.xlen) + ' ' + test + ' -o ' + elf

            #TODO: we are using -D to enable compile time macros. If your
            #      toolchain is not riscv-gcc you may want to change the below code
            compile_cmd = cmd + ' -D' + " -D".join(testentry['macros'])
            execute+=compile_cmd+";"

            execute += self.objcopy
            execute += self.bin2hex + ' -w 64 -i ' + elf + '.bin' + ' -o ' + elf + '.hex64;'
            execute += self.readelf
            execute += self.objdump


            sig_file = os.path.join(test_dir, self.name[:-1] + ".signature")

            #execute += self.dut_exe + '' 

            make.add_target(execute)
        make.execute_all(self.work_dir)
