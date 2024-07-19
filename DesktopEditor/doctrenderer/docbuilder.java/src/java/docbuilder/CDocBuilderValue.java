package docbuilder;

public class CDocBuilderValue {
    private long c_internal = 0;

    // NATIVE METHODS:
    private native long c_Create();
    private native long c_Copy(long other);
    private native void c_Destroy(long self);

    private native boolean c_IsEmpty(long self);
    private native void c_Clear(long self);

    private native boolean c_IsNull(long self);
    private native boolean c_IsUndefined(long self);
    private native boolean c_IsInt(long self);
    private native boolean c_IsDouble(long self);
    private native boolean c_IsString(long self);
    private native boolean c_IsFunction(long self);
    private native boolean c_IsObject(long self);
    private native boolean c_IsArray(long self);

    private native int c_GetLength(long self);

    private native boolean c_ToBool(long self);
    private native int c_ToInt(long self);
    private native double c_ToDouble(long self);
    private native String c_ToString(long self);

    private native long c_GetProperty(long self, String name);
    private native long c_GetByIndex(long self, int index);

    private native void c_SetProperty(long self, String name, long value);
    private native void c_SetByIndex(long self, int index, long value);

    private native long c_CreateWithBool(boolean value);
    private native long c_CreateWithInt(int value);
    private native long c_CreateWithDouble(double value);
    private native long c_CreateWithString(String value);

    private native long c_CreateUndefined();
    private native long c_CreateNull();
    private native long c_CreateArray(int length);

    private native long c_Call0(long self, String name);
    private native long c_Call1(long self, String name, long p1);
    private native long c_Call2(long self, String name, long p1, long p2);
    private native long c_Call3(long self, String name, long p1, long p2, long p3);
    private native long c_Call4(long self, String name, long p1, long p2, long p3, long p4);
    private native long c_Call5(long self, String name, long p1, long p2, long p3, long p4, long p5);
    private native long c_Call6(long self, String name, long p1, long p2, long p3, long p4, long p5, long p6);
}
