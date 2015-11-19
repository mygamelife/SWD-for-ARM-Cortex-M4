PROJECT_CEEDLING_ROOT = "vendor/ceedling"
load "#{PROJECT_CEEDLING_ROOT}/lib/ceedling.rb"

# load File.join(File.dirname('./scripts'), 'cbuild.rb')
load "scripts/helper.rb"

# With this now we can choose which '.yml' configuration file to be loaded by
# issuing e.g.:  
#     rake project=./myproject.yml test:all
#
project_file = get_value_from_env('p', './project.yml')

Ceedling.load_project(config: project_file)
load_extra_scripts(project_file)

desc 'Build, Tests and deploy'
task :build_tests_and_deploy do
  # Configure appropriate .yml and run probe test
  system "rake p=probe.yml probe:test:all"
  # Run host test
  system "rake host:test:all"
  # Configure appropriate .yml and run hard-in-the-loop test
  system "rake p=probe_hil.yml probe:hil_test:all"
end

task :default => %w[ build_tests_and_deploy]
# task :default => %w[ build_tests_and_deploy test:all]