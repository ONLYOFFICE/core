package docbuilder;

public class CDocBuilderContext {
    public CDocBuilderContext() {
        c_internal = 0;
    }

    CDocBuilderContext(long internal) {
        c_internal = internal;
    }

    long c_internal = 0;
}
