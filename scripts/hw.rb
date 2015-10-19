#FLASHER = "\"/C/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI\" "
FLASHER = "ST-LINK_CLI " unless defined? FLASHER
C_EXCEPTION_PATH = "vendor/ceedling/vendor/c_exception/lib " unless defined? C_EXCEPTION_PATH

# Load build script to help build C program
load "scripts/cbuild.rb"

# Configuration parameters
config = {
  :verbose      => :no,
  :compiler     => 'arm-none-eabi-gcc',
  :linker       => 'arm-none-eabi-gcc',
# -IC:\Users\user26\CoIDE\workspace\RTOS
  :include_path => ['app/Hardware', 'app/Tlv',
                    C_EXCEPTION_PATH,
                    'app'],
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

namespace :hw do
  task :prepare_release do
    filenames = getAllSrcFiles("SWD-for-ARM-Cortex-M4.coproj")
    dep_list = createCompilationDependencyList(filenames, ['c', '.c++', '.s', 'cpp', 'asm'], '.', '.o')
    dep_list = compile_list(dep_list, '.', 'build/release/hw', '.', config)
    link_all(getDependers(dep_list), 'build/release/hw/SWD-for-ARM-Cortex-M4.elf', config)

    file 'build/release/hw/SWD-for-ARM-Cortex-M4.hex' => 'build/release/hw/SWD-for-ARM-Cortex-M4.elf' do |n|
      puts "converting #{n.prerequisites[0]} to hex..."
      system "arm-none-eabi-objcopy -O ihex #{n.prerequisites[0]} #{n.name}"
    end
  end
  CLEAN.include('build/release/hw') if File.exist? 'build/release/hw'

  desc 'Build hardware release code'
  task :release => :prepare_release do
  #  p Rake.application.tasks
    Rake::Task["build/release/hw/SWD-for-ARM-Cortex-M4.elf"].invoke
  end

  desc 'Run hardware-in-the-loop test'
  task :test => :prepare_release do
    Rake::test["Host/test/test_hwTlv"].invoke
  end
  
  desc 'Flash program and run test'
  task :flash => :prepare_release do
    Rake::Task["build/release/hw/SWD-for-ARM-Cortex-M4.hex"].invoke
    system (FLASHER + '-P build/release/hw/SWD-for-ARM-Cortex-M4.hex -V while_programming -Rst -Run')
  end
end
