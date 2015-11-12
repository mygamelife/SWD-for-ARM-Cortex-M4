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

task :default => %w[ test:all release ]