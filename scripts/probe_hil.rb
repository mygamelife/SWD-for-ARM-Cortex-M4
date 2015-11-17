load "scripts/probe_base.rb"
load "scripts/target.rb"

# This is to build target flash programmer code, build probe released code and flash the probe
desc 'Build target and flash probe'
task :build_taget_and_flash_probe => ['target:release', 'probe:flash']
# This is the abbreviation of the above task
task :bf => [:build_taget_and_flash_probe]
namespace :test do
  filenames = get_all_tests("test/Hardware/**/test_*.c")
  # task :all => (['probe:flash'] + filenames)
  # task :all => "target:release[FlashProgrammer/FlashProgrammer.coproj]"
  # task :all => (['probe:flash', 'target:release'] + filenames)
  desc 'Run all hardware-in-the-loop tests'
  task :all => ([:bf] + filenames)
end
