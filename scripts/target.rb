#FLASHER = "\"/C/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI\" "
FLASHER = "ST-LINK_CLI " unless defined? FLASHER

# Load build script to help build C program
load "scripts/cbuild.rb"

# Main dependency list
# main_dependency = {
  # depender                dependees
  # 'ErrorCode.o'       => ['ErrorCode.c', 'ErrorCode.o'],
  # 'FileToken.o'       => ['FileToken.c', 'FileToken.h', 'Token.h', 'StringObject.h'],
  # 'GetHeaders.o'      => ['GetHeaders.c', 'GetHeaders.h', 'Read_File.h', 'elf.h', 'ProgramElf.h'],
  # 'IdentifierToken.o' => ['IdentifierToken.c', 'IdentifierToken.h', 'Token.h', 'StringObject.h'],
  # 'Interface.o'       => ['Interface.c', 'Interface.h', 'Token.h', 'Tlv.h', 'ErrorCode.h'],
  # 'MemoryReadWrite.o' => ['MemoryReadWrite.c', 'MemoryReadWrite.h', 'Yield.h', 'ProgramLoader.h'],
  # 'NumberToken.o'     => ['NumberToken.c', 'NumberToken.h', 'Token.h', 'StringObject.h'],
  # 'OperatorToken.o'   => ['OperatorToken.c', 'OperatorToken.o', 'ErrorCode.h', 'StringObject.h'],
  # 'ProgramLoader.o'   => ['ProgramLoader.c', 'ProgramLoader.h', 'Tlv.h', 'GetHeaders.h', 'ErrorCode.h',
                          # 'Interface.h', 'SystemTime.h'],
  # 'Read_File.o'       => ['Read_File.c', 'Read_File.h', 'elf.h'],
  # 'Relocator.o'       => ['Relocator.c', 'Relocator.h', 'Read_File.h', 'elf.h', 'ProgramElf.h', 'GetHeaders.h'],
  # 'StringObject.o'    => ['StringObject.c', 'StringObject.h'],
  # 'SystemTime.o'      => ['SystemTime.c', 'SystemTime.h'],
  # 'Tlv.o'             => ['Tlv.c', 'Tlv.h', 'Uart.h', 'TlvEx.h', 'ErrorCode.h', 'SystemTime.h', 'Yield.h'],
  # 'Token.o'           => ['Token.c', 'Token.h', 'StringObject.h', 'NumberToken.h', 'OperatorToken.h',
                          # 'IdentifierToken.h', 'FileToken.h', 'ErrorCode.h'],
  # 'Uart.o'            => ['Uart.c', 'Uart.h', 'ErrorCode.h'],
# }

# Support library dependency list
# exception_dependency = {'CException.o'  => ['CException.c', 'CException.h']}

# Support dependency list
# header_dependency = {
  # 'FileToken.o'       => ['Token.h', 'StringObject.h'],
  # 'GetHeaders.o'      => ['Read_File.h', 'elf.h', 'ProgramElf.h'],
  # 'IdentifierToken.o' => ['Token.h', 'StringObject.h'],
  # 'Interface.o'       => ['Token.h', 'Tlv.h', 'ErrorCode.h'],
  # 'MemoryReadWrite.o' => ['Yield.h', 'ProgramLoader.h'],
  # 'OperatorToken.o'   => ['ErrorCode.h', 'StringObject.h'],
  # 'NumberToken.o'     => ['Token.h', 'StringObject.h'],
  # 'ProgramLoader.o'   => ['Tlv.h', 'GetHeaders.h', 'ErrorCode.h', 'Interface.h', 'SystemTime.h'],
  # 'Read_File.o'       => ['elf.h'],
  # 'Relocator.o'       => ['Read_File.h', 'elf.h', 'ProgramElf.h', 'GetHeaders.h'],
  # 'Token.o'           => ['StringObject.h', 'NumberToken.h', 'OperatorToken.h',
                          # 'IdentifierToken.h', 'FileToken.h', 'ErrorCode.h'],
  # 'Tlv.o'             => ['Uart.h', 'TlvEx.h', 'ErrorCode.h', 'SystemTime.h', 'Yield.h'],
  # 'Uart.o'            => ['ErrorCode.h'],
# }

# Configuration parameters
config = {
  :verbose      => :no,
  :compiler     => 'arm-none-eabi-gcc',
  :linker       => 'arm-none-eabi-gcc',
# -IC:\Users\user26\CoIDE\workspace\RTOS
  :include_path => ['src/app/Stub', 'src/app/SystemConfig',
                    'FlashProgrammer/app/Drivers',
                    'FlashProgrammer/app/Legacy',
                    'FlashProgrammer/app'],
  :user_define  => ['STM32F429ZI', 'STM32F429xx'],
  :library_path => '.',
#  :library => ['libusb'],
  :linker_script => 'FlashProgrammer/ToRam.ld',
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

# namespace :target do
  # desc 'Build target release code'
  # task :release do
               # dependency list  directory of   directory of     directory of    config
                                # dependee       .o files         .exe            object
    # compile_list(main_dependency, 'src/app/Tlv', 'build/release/host/c', 'build/release', config)
    # compile_list(exception_dependency, CEXCEPTION_PATH, 'build/release/host/c', 'build/release', config)
    # compile_list(header_dependency, 'src', 'src', 'build/release', config)
   # p Rake.application.tasks
    # Rake::Task["build/release/Main.exe"].invoke
  # end
# end

namespace :target do
  task :prepare_release do
    dep_list = compile_all(['src/app/Stub', 'src/app/SystemConfig', 
                            'FlashProgrammer/app/Drivers',
                            'FlashProgrammer/app/Legacy',
                            'FlashProgrammer/app'],
                            'build/release/target/c',
                            config)
    link_all(getDependers(dep_list), 'build/release/target/FlashProgrammer.elf', config)
  end
  CLEAN.include('build/release/target') if File.exist? 'build/release/target'

  desc 'Build flash programmer release code'
  task :release => :prepare_release do
   # p Rake.application.tasks
    Rake::Task["build/release/target/FlashProgrammer.elf"].invoke
  end
end