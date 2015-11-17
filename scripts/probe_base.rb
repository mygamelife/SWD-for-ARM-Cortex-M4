require 'mkmf'
# Load cbuild script to help build C program
load "scripts/cbuild.rb"

FLASHER = trim_string((flasher = ENV['flasher']) ? String.new(flasher):"ST-LINK_CLI") unless defined? FLASHER
ELF_TO_HEX = trim_string((elf_to_hex = ENV['elf_to_hex']) ? String.new(elf_to_hex):"arm-none-eabi-objcopy") unless defined? ELF_TO_HEX
C_EXCEPTION_PATH = "vendor/ceedling/vendor/c_exception/lib " unless defined? C_EXCEPTION_PATH

# ST-LINK_CLI error code. They can be found in [1].
ERR_ST_LINK_ARGS = 1
ERR_ST_LINK_CONNECTION = 2
ERR_ST_LINK_COMMAND = 3
ERR_ST_LINK_RESET = 7
ERR_ST_LINK_HALT = 8
ERR_ST_LINK_STEP = 9
ERR_ST_LINK_BREAKPOINT = 10
ERR_ST_LINK_ERASE_FLASH = 11
ERR_ST_LINK_PROG_VERIFY = 12

# Configuration parameters
config = {
  :verbose      => :no,
  :compiler     => 'arm-none-eabi-gcc',
  :linker       => 'arm-none-eabi-gcc',
# -IC:\Users\user26\CoIDE\workspace\RTOS
  :include_path => [C_EXCEPTION_PATH,
                    'src/app/Drivers',
                    'src/app/Stub',
                    'src/app/Tlv',
                    'src/app/SystemConfig',
                    'src/app/PlatformSpecific',
                    'src/app'],
  :user_define  => ['STM32F429ZI', 'STM32F429xx'],
  :library_path => '.',
#  :library => ['libusb'],
  :linker_script => 'STM32F429ZI_FLASH.ld',
  :compiler_options => ['-mcpu=cortex-m4 -mthumb -g2 -Wall -O0 -g'],          # Other compiler options
  :linker_options => ['-mcpu=cortex-m4 -mthumb -g2 -O0 -Wl,--gc-sections'],   # Other linker options
  :option_keys  => {:library => '-l',
                    :library_path => '-L',
                    :include_path => '-I',
                    :output_file => '-o',
                    :compile => '-c',
                    :linker_script => '-Wl,-T',
                    :define => '-D'}
}

namespace :probe do
  OUTPUT_PATH = 'build/release/probe/' unless defined? OUTPUT_PATH
  ouput_elf = nil
  ouput_hex = nil
  task :prepare_release, [:coproj] do |t, args|
    filenames, coproj = get_all_source_files_in_coproj(args[:coproj])
    puts "Building sources in #{coproj}..."
    file = File.basename(coproj, '.*')
    ouput_elf = File.join(OUTPUT_PATH, file + '.elf')
    ouput_hex = File.join(OUTPUT_PATH, file + '.hex')
    dep_list = createCompilationDependencyList(filenames, ['c', '.c++', '.s', 'cpp', 'asm'], '.', '.o')
    dep_list = compile_list(dep_list, '.', OUTPUT_PATH, '.', config)
  #  p dep_list
    link_all(getDependers(dep_list), ouput_elf, config)

    file ouput_hex => ouput_elf do |n|
      if(program_available?(ELF_TO_HEX) == nil)
        puts "Error: Cannot find #{ELF_TO_HEX} program to turn ELF to HEX."
        exit
      end
      puts "converting #{n.prerequisites[0]} to hex..."
      sys_cli "#{ELF_TO_HEX} -O ihex #{n.prerequisites[0]} #{n.name}"
    end
  end
#  CLEAN.include('build/release/hw') if File.exist? 'build/release/hw'

  desc 'Build hardware release code'
  task :release, [:coproj] => :prepare_release do |t, args|
  #  p Rake.application.tasks

    Rake::Task[ouput_elf].invoke(args)
  #  sh "cp #{OUTPUT_FILE} ."
  end

  desc 'Flash program and run test'
  task :flash, [:coproj] => :prepare_release do |t, args|
    Rake::Task[ouput_hex].invoke(args)
    if(program_available?(FLASHER) == nil)
      puts "Error: Cannot find #{FLASHER} program to flash ARM processor."
      exit
    end
    # First check if there is any differences between current Hex file with
    # the one on the MCU Flash. Download if there is, otherwise do nothing.
    if (!system "#{FLASHER} -CmpFile #{ouput_hex}") && ($?.exitstatus != ERR_ST_LINK_CONNECTION)
      # Flash the Hex file into the MCU Flash
      sys_cli "#{FLASHER} -P #{ouput_hex} -V while_programming -Rst -Run"
    end
  end

  desc 'Erase all Flash sectors'
  task :full_erase do
    sys_cli "#{FLASHER} -ME"
  end  
  
end

namespace :probe do
  namespace :test do
    filenames = get_all_tests("test/Probe/**/test_*.c")
    desc 'Run all probe tests'
    task :all => filenames
  end
end