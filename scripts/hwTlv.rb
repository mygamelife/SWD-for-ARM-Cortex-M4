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

namespace :hwTlv do
  task :prepare_release do
    filenames = getAllSrcFiles("Host/test/HardwareTestProj/TlvService.coproj")
    dep_list = createCompilationDependencyList(filenames, ['c', '.c++', '.s', 'cpp', 'asm'], '.', '.o')
    dep_list = compile_list(dep_list, '.', 'build/release/hwTlv', '.', config)
    link_all(getDependers(dep_list), 'build/release/hwTlv/TlvService.elf', config)

    file 'build/release/hwTlv/TlvService.hex' => 'build/release/hwTlv/TlvService.elf' do |n|
      puts "converting #{n.prerequisites[0]} to hex..."
      system "arm-none-eabi-objcopy -O ihex #{n.prerequisites[0]} #{n.name}"
    end
  end
  CLEAN.include('build/release/hwTlv') if File.exist? 'build/release/hwTlv'

  desc 'Build hardware release code'
  task :release => :prepare_release do
  #  p Rake.application.tasks
    Rake::Task["build/release/hwTlv/TlvService.elf"].invoke
  end


  desc 'Flash program and run test'
  task :flash => :prepare_release do
    Rake::Task["build/release/hwTlv/TlvService.hex"].invoke
    system (FLASHER + '-P build/release/hwTlv/TlvService.hex -V while_programming -Rst -Run')
  end
end
