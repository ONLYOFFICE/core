package docbuilder;

public class CDocBuilderContextScope {
    public CDocBuilderContextScope() {
        c_internal = 0;
    }

    CDocBuilderContextScope(long internal) {
        c_internal = internal;
    }

    long c_internal = 0;
}
