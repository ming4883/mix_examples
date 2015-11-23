package org.mix.example_02;

public class MainActivity extends org.mix.common.BaseActivity {

	@Override
    protected String[] getLibraries() {
        return new String[] {
            "gnustl_shared",
            "example_02"
        };
    }
}
