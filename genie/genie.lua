MIX_DIR = path.getabsolute ("../../mix")
PROJECT_DIR = path.getabsolute ("../")

solution "mix_examples"
	dofile (path.join (MIX_DIR, "genie/setup.lua"))
	
	if mix_is_android() then
		gradle.buildscript:repositories {
			--"maven { url 'https://maven.fabric.io/public' }",
		}
		
		gradle.buildscript:dependencies {
			--"classpath 'io.fabric.tools:gradle:1.+'",
		}
	end
	
	function add_example (example_name)
		local example_dir = path.join (PROJECT_DIR, "src", example_name)
		
		project (example_name)
		
		mix_setup_common_app ()
		
		files {
			path.join (example_dir, "app.cpp"),
		}
		
		if mix_is_ios() then
			files {
				path.join (example_dir, "ios/info.plist"),
				path.join (PROJECT_DIR, "src/common/ios/LaunchScreen.xib"),
			}
		end
		
		if mix_is_android() then
			local grd_prj = gradle:project()
			
			grd_prj.manifest = path.join (example_dir, "android/AndroidManifest.xml")
			
			grd_prj.multiDexEnabled = true
			
			grd_prj:java_srcdirs {
				path.join (MIX_DIR, "src/mix/android/app/java"),
				path.join (example_dir, "android/java")
			}
			
			grd_prj:res_srcdirs {
				path.join (example_dir, "android/res")
			}
			
			grd_prj:buildType("release").minifyEnabled = true
			grd_prj:buildType("release").proguardFiles = {
				"getDefaultProguardFile('proguard-android.txt')"
			}
		end
	end
	
	add_example ("example_00")
		if mix_is_android() then
			local grd_prj = gradle:project()
			
			grd_prj:plugins {
				-- "io.fabric"
			}
			
			grd_prj:dependencies {
				"compile 'com.google.android.gms:play-services-games:7.3.0'"
			}
			
			grd_prj:extras {
				"// this line is a test grd_prj:extras()"
			}
			
			grd_prj:ndk_extras {
				"# this line is a test grd_prj:ndk_extras()"
			}
			
			grd_prj.externalprojects["dummy"] = path.join (PROJECT_DIR, "vendor/dummy")
		end
		
		
	add_example ("example_01")
	
	mix_add_zlib_project()
	
	mix_add_unit_tests_project()
	
	startproject ("example_00")
	
	--defines ({"TEST_COMMON=1", "TEST_APP=1"})