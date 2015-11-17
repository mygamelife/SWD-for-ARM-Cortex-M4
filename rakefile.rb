PROJECT_CEEDLING_ROOT = "vendor/ceedling"
load "#{PROJECT_CEEDLING_ROOT}/lib/ceedling.rb"

load "scripts/helper.rb"

# With this now we can choose which '.yml' configuration file to be loaded by
# issuing e.g.:  
#     rake project=./myproject.yml test:all
#
project_file = (project_file = ENV['p']) ? String.new(project_file):'./project.yml'
raise ArgumentError, "Error: #{project_file} folder does not exist."  \
                                          if !File.exist?(project_file)
Ceedling.load_project(config: project_file)
load_extra_scripts(project_file)

desc 'Build, Tests and deploy'
task :build_tests_and_deploy do
  # Build and deploy
  system "rake p=probe.yml probe:test:all"
  system "rake host:test:all"
  system "rake p=probe_hil.yml probe:hw:test:all"

end

task :default => %w[ build_tests_and_deploy]
# task :default => %w[ build_tests_and_deploy test:all]