# Load build script to help build C program
load File.join(File.dirname(__FILE__), 'cbuild.rb')

elf_to_hex = get_value_from_env("elf_to_hex", "arm-none-eabi-objcopy")
c_exception_path = "vendor/ceedling/vendor/c_exception/lib"

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

namespace :target do
  target_output_path = 'build/release/target/'
  ouput_elf = nil
  ouput_hex = nil
  task :prepare_release, [:coproj] do |t, args|
    filenames, coproj = get_all_source_files_in_coproj(args[:coproj], './FlashProgrammer')
    file = File.basename(coproj, '.coproj')
    ouput_elf = File.join(target_output_path, file + '.elf')
    ouput_hex = File.join(target_output_path, file + '.hex')
    dep_list = createCompilationDependencyList(filenames, ['c', '.c++', '.s', 'cpp', 'asm'], '.', '.o')
    dep_list = compile_list(dep_list, '.', target_output_path, '.', config)
  #  p dep_list
    link_all(getDependers(dep_list), ouput_elf, config)

    file ouput_hex => ouput_elf do |n|
      if(program_available?(elf_to_hex) == nil)
        puts "Error: Cannot find #{elf_to_hex} program to turn ELF to HEX."
        exit
      end
      puts "converting #{n.prerequisites[0]} to hex..."
      sys_cli "#{elf_to_hex} -O ihex #{n.prerequisites[0]} #{n.name}"
    end
  end

  desc 'Build probe hardware release code'
  task :release, [:coproj] => :prepare_release do |t, args|
    Rake::Task[ouput_elf].invoke(args)
  end
end