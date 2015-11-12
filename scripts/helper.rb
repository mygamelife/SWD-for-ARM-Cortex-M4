require 'yaml'

def load_extra_scripts(project_file)
  content = YAML.load_file project_file
  scripts_to_load = content[:project][:load_scripts]
  if scripts_to_load
    if scripts_to_load.is_a? Array
      scripts_to_load.each { |script|
        load script
      }
    else
      load scripts_to_load
    end
  end 
end

def get_value_from_env(name, default_value)
  trim_string((flasher = ENV[name]) ? String.new(flasher):default_value)
end

def trim_string(str)
  return nil if str == nil
  str.gsub!(/^\s*/, "").gsub!(/\s*$/, "")
end