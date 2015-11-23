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
		
		mix_setup_app()
	
		files {
			path.join (MIX_DIR, "include/mix/*.h"),
			path.join (MIX_DIR, "src/mix/*.cpp"),
			path.join (example_dir, "app.cpp"),
		}
		
		excludes {
			path.join (MIX_DIR, "src/mix/mix_tests*"),
		}
	
		includedirs {
			path.join (MIX_DIR, "include/"),
		}
	
		links {
			"bgfx_static",
		}
		
		if mix_is_windows_desktop() then
			defines { "MIX_WINDOWS_DESKTOP" }
		end
		
		if mix_is_android() then
			defines { "MIX_ANDROID" }
		
			local grd_prj = gradle:project()
			
			local asset_dir = path.join ("../runtime/", project().name, "android")
			if os.isdir (asset_dir) then
				grd_prj:assets_srcdirs {asset_dir}
			end
			
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
		
		local add_runtime_zip_file = function (platform)
			local runtime_file = path.join ("../runtime/", project().name, platform, "runtime.zip");
			if os.isfile (runtime_file) then
				files {runtime_file}
			end
		end
	
		if mix_is_ios() then
			files {
				path.join (MIX_DIR, "src/mix/*ios.mm"),
				path.join (example_dir, "ios/info.plist"),
				path.join (PROJECT_DIR, "src/common/ios/LaunchScreen.xib"),
			}
			--local runtime_file = path.join ("../runtime/", project().name, "ios/runtime.zip");
			--if os.isfile (runtime_file) then
			--	files {runtime_file}
			--end
			add_runtime_zip_file ("ios")
		
			defines { "MIX_IOS" }
		
			buildoptions {
				"-fobjc-arc"
			}
		end
	
		if mix_is_tvos() then
			files {
				path.join (MIX_DIR, "src/mix/*tvos.mm"),
				path.join (example_dir, "tvos/info.plist"),
			}
			--local runtime_file = path.join ("../runtime/", project().name, "ios/runtime.zip");
			--if os.isfile (runtime_file) then
			--	files {runtime_file}
			--end
			add_runtime_zip_file ("ios")
		
			defines { "MIX_TVOS" }
		
			buildoptions {
				"-fobjc-arc"
			}
		end
	
		if mix_is_osx() then
			files { path.join (MIX_DIR, "src/mix/*osx.mm") }
			--local runtime_file = path.join ("../runtime/", project().name, "osx/runtime.zip");
			--if os.isfile (runtime_file) then
			--	files {runtime_file}
			--end
			add_runtime_zip_file ("osx")
			
			defines { "MIX_OSX" }
		
			buildoptions {
				"-fobjc-arc"
			}
		end
	
		mix_use_zlib()
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
	
	add_example ("example_02")
	
	mix_add_zlib_project()
	
	mix_add_unit_tests_project()
	
	startproject ("example_00")
	
	--defines ({"TEST_COMMON=1", "TEST_APP=1"})