local MIX_COMMON_DIR = path.getabsolute ("../../mix")
local PROJECT_DIR = path.getabsolute ("../")

solution "mix_examples"
	dofile (path.join (MIX_COMMON_DIR, "genie/setup.lua"))
	
	if mix_is_android() then
		premake.gradle.appabi = {"armeabi", "armeabi-v7a", "x86"}
		
		premake.gradle.buildscript.repositories = {
			--"maven { url 'https://maven.fabric.io/public' }"
		}
		
		premake.gradle.buildscript.dependencies = {
			--"classpath 'io.fabric.tools:gradle:1.+'"
		}
		
		premake.gradle.ndk.ld_gold.enable = true
		premake.gradle.ndk.ld_gold.multithread = true
		
	end
	
	project ("example_00")
		mix_setup_common_app ()
		
		files {
			path.join (PROJECT_DIR, "src/example_00/app.cpp"),
		}
			
		if mix_is_ios() then
			files {
				path.join (PROJECT_DIR, "src/example_00/ios/info.plist"),
				path.join (PROJECT_DIR, "src/common/ios/LaunchScreen.xib"),
			}
		end
		
		if mix_is_android() then
			local grd = gradle()
			grd.appabi = {"armeabi", "armeabi-v7a", "x86"}
			
			grd.manifest = path.join (PROJECT_DIR, "src/example_00/android/AndroidManifest.xml")
			
			grd.java_srcdirs = {
				path.join (MIX_COMMON_DIR, "src/mix/android/app/java"),
				path.join (PROJECT_DIR, "src/example_00/android/java")
			}
			
			grd.res_srcdirs = {
				path.join (PROJECT_DIR, "src/example_00/android/res")
			}
			
			grd.plugins = {
				-- "io.fabric"
			}
			
			grd.dependencies = {
				"compile 'com.google.android.gms:play-services-games:7.3.0'"
			}
			
			grd.externalprojects["dummy"] = path.join (PROJECT_DIR, "vendor/dummy")
			
			grd.buildTypes.release.minifyEnabled = true
			grd.buildTypes.release.proguardFiles = {
				"getDefaultProguardFile('proguard-android.txt')"
			}
			
			grd.multiDexEnabled = true
		end
	
	mix_common_tests_project()
	
	startproject ("example_00")
	
	--defines ({"TEST_COMMON=1", "TEST_APP=1"})