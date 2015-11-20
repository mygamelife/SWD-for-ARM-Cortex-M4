load File.join(File.dirname(__FILE__), 'probe_base.rb')

namespace :probe do
  namespace :test do
    filenames = get_all_tests("test/Probe/**/test_*.c")
    desc 'Run all probe tests'
    task :all => filenames
  end
end