load File.join(File.dirname(__FILE__), 'probe_base.rb')
load File.join(File.dirname(__FILE__), 'target.rb')
# load "scripts/probe_base.rb"
# load "scripts/target.rb"

# This is to build target flash programmer code, build probe released code and flash the probe
desc 'Build target and flash probe'
task :build_taget_and_flash_probe => ['target:release', 'probe:flash']
# This is the abbreviation of the above task
task :bf => [:build_taget_and_flash_probe]
namespace :probe do
  namespace :hil_test do
    filenames = get_all_tests("test/Hardware/**/test_HIL_*.c")
    desc 'Run all hardware-in-the-loop tests'
    task :all => ([:bf] + filenames)
  end
end