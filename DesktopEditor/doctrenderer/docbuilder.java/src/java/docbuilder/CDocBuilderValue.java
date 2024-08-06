package docbuilder;

public class CDocBuilderValue {
    public CDocBuilderValue() {
        c_internal = c_Create();
    }

    CDocBuilderValue(long internal) {
        c_internal = internal;
    }

    public CDocBuilderValue(CDocBuilderValue other) {
        c_internal = c_Copy(other.c_internal);
    }

    protected void finalize()
    {
        c_Destroy(c_internal);
    }

    public boolean isEmpty()
    {
        return c_IsEmpty(c_internal);
    }

    public void clear()
    {
        c_Clear(c_internal);
    }

    public boolean isNull()
    {
        return c_IsNull(c_internal);
    }

    public boolean isUndefined()
    {
        return c_IsUndefined(c_internal);
    }

    public boolean isInt()
    {
        return c_IsInt(c_internal);
    }

    public boolean isDouble()
    {
        return c_IsDouble(c_internal);
    }

    public boolean isString()
    {
        return c_IsString(c_internal);
    }

    public boolean isFunction()
    {
        return c_IsFunction(c_internal);
    }

    public boolean isObject()
    {
        return c_IsObject(c_internal);
    }

    public boolean isArray()
    {
        return c_IsArray(c_internal);
    }

    public int getLength()
    {
        return c_GetLength(c_internal);
    }

    public boolean toBool()
    {
        return c_ToBool(c_internal);
    }

    public int toInt()
    {
        return c_ToInt(c_internal);
    }

    public double toDouble()
    {
        return c_ToDouble(c_internal);
    }

    public String toString()
    {
        return c_ToString(c_internal);
    }

    public CDocBuilderValue getProperty(String name) {
        return new CDocBuilderValue(c_GetProperty(c_internal, name));
    }

    public CDocBuilderValue get(String name) {
        return new CDocBuilderValue(c_GetProperty(c_internal, name));
    }

    public CDocBuilderValue get(int index) {
        return new CDocBuilderValue(c_GetByIndex(c_internal, index));
    }

    public void setProperty(String name, Object value) {
        CDocBuilderValue docBuilderValue = getValueFromObject(value);
        c_SetProperty(c_internal, name, docBuilderValue.c_internal);
    }

    public void set(String name, Object value) {
        CDocBuilderValue docBuilderValue = getValueFromObject(value);
        c_SetProperty(c_internal, name, docBuilderValue.c_internal);
    }

    public void set(int index, Object value) {
        CDocBuilderValue docBuilderValue = getValueFromObject(value);
        c_SetByIndex(c_internal, index, docBuilderValue.c_internal);
    }

    public CDocBuilderValue(boolean value) {
        c_internal = c_CreateWithBool(value);
    }

    public CDocBuilderValue(int value) {
        c_internal = c_CreateWithInt(value);
    }

    public CDocBuilderValue(double value) {
        c_internal = c_CreateWithDouble(value);
    }

    public CDocBuilderValue(String value) {
        c_internal = c_CreateWithString(value);
    }

    public CDocBuilderValue(Object[] values) {
        int length = values.length;
        c_internal = c_CreateArray(length);
        for (int i = 0; i < length; i++) {
            this.set(i, getValueFromObject(values[i]));
        }
    }

    static CDocBuilderValue getValueFromObject(Object obj) {
        if (obj instanceof CDocBuilderValue) {
            return (CDocBuilderValue)obj;
        } else if (obj instanceof Boolean) {
            return new CDocBuilderValue((Boolean)obj);
        } else if (obj instanceof Integer) {
            return new CDocBuilderValue((Integer)obj);
        } else if (obj instanceof Double) {
            return new CDocBuilderValue((Double)obj);
        } else if (obj instanceof String) {
            return new CDocBuilderValue((String)obj);
        } else if (obj instanceof Object[]) {
            return new CDocBuilderValue((Object[])obj);
        } else {
            throw new IllegalArgumentException("Unsupported type for CDocBuilderValue");
        }
    }

    public static CDocBuilderValue createUndefined() {
        return new CDocBuilderValue(c_CreateUndefined());
    }

    public static CDocBuilderValue createNull() {
        return new CDocBuilderValue(c_CreateNull());
    }

    public static CDocBuilderValue createArray(int length) {
        return new CDocBuilderValue(c_CreateArray(length));
    }

    public CDocBuilderValue call(String name) {
        return new CDocBuilderValue(c_Call0(c_internal, name));
    }

    public CDocBuilderValue call(String name, Object p1) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        return new CDocBuilderValue(c_Call1(c_internal, name, pValue1.c_internal));
    }

    public CDocBuilderValue call(String name, Object p1, Object p2) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        CDocBuilderValue pValue2 = getValueFromObject(p2);
        return new CDocBuilderValue(c_Call2(c_internal, name, pValue1.c_internal, pValue2.c_internal));
    }

    public CDocBuilderValue call(String name, Object p1, Object p2, Object p3) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        CDocBuilderValue pValue2 = getValueFromObject(p2);
        CDocBuilderValue pValue3 = getValueFromObject(p3);
        return new CDocBuilderValue(c_Call3(c_internal, name, pValue1.c_internal, pValue2.c_internal, pValue3.c_internal));
    }

    public CDocBuilderValue call(String name, Object p1, Object p2, Object p3, Object p4) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        CDocBuilderValue pValue2 = getValueFromObject(p2);
        CDocBuilderValue pValue3 = getValueFromObject(p3);
        CDocBuilderValue pValue4 = getValueFromObject(p4);
        return new CDocBuilderValue(c_Call4(c_internal, name, pValue1.c_internal, pValue2.c_internal, pValue3.c_internal, pValue4.c_internal));
    }

    public CDocBuilderValue call(String name, Object p1, Object p2, Object p3, Object p4, Object p5) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        CDocBuilderValue pValue2 = getValueFromObject(p2);
        CDocBuilderValue pValue3 = getValueFromObject(p3);
        CDocBuilderValue pValue4 = getValueFromObject(p4);
        CDocBuilderValue pValue5 = getValueFromObject(p5);
        return new CDocBuilderValue(c_Call5(c_internal, name, pValue1.c_internal, pValue2.c_internal, pValue3.c_internal, pValue4.c_internal, pValue5.c_internal));
    }

    public CDocBuilderValue call(String name, Object p1, Object p2, Object p3, Object p4, Object p5, Object p6) {
        CDocBuilderValue pValue1 = getValueFromObject(p1);
        CDocBuilderValue pValue2 = getValueFromObject(p2);
        CDocBuilderValue pValue3 = getValueFromObject(p3);
        CDocBuilderValue pValue4 = getValueFromObject(p4);
        CDocBuilderValue pValue5 = getValueFromObject(p5);
        CDocBuilderValue pValue6 = getValueFromObject(p6);
        return new CDocBuilderValue(c_Call6(c_internal, name, pValue1.c_internal, pValue2.c_internal, pValue3.c_internal, pValue4.c_internal, pValue5.c_internal, pValue6.c_internal));
    }

    // Native code
    static {
        docbuilder.utils.NativeLibraryLoader.loadLibraries();
    }

    long c_internal = 0;

    private static native long c_Create();
    private static native long c_Copy(long other);
    private static native void c_Destroy(long self);

    private static native boolean c_IsEmpty(long self);
    private static native void c_Clear(long self);

    private static native boolean c_IsNull(long self);
    private static native boolean c_IsUndefined(long self);
    private static native boolean c_IsInt(long self);
    private static native boolean c_IsDouble(long self);
    private static native boolean c_IsString(long self);
    private static native boolean c_IsFunction(long self);
    private static native boolean c_IsObject(long self);
    private static native boolean c_IsArray(long self);

    private static native int c_GetLength(long self);

    private static native boolean c_ToBool(long self);
    private static native int c_ToInt(long self);
    private static native double c_ToDouble(long self);
    private static native String c_ToString(long self);

    private static native long c_GetProperty(long self, String name);
    private static native long c_GetByIndex(long self, int index);

    private static native void c_SetProperty(long self, String name, long value);
    private static native void c_SetByIndex(long self, int index, long value);

    private static native long c_CreateWithBool(boolean value);
    private static native long c_CreateWithInt(int value);
    private static native long c_CreateWithDouble(double value);
    private static native long c_CreateWithString(String value);

    private static native long c_CreateUndefined();
    private static native long c_CreateNull();
    private static native long c_CreateArray(int length);

    private static native long c_Call0(long self, String name);
    private static native long c_Call1(long self, String name, long p1);
    private static native long c_Call2(long self, String name, long p1, long p2);
    private static native long c_Call3(long self, String name, long p1, long p2, long p3);
    private static native long c_Call4(long self, String name, long p1, long p2, long p3, long p4);
    private static native long c_Call5(long self, String name, long p1, long p2, long p3, long p4, long p5);
    private static native long c_Call6(long self, String name, long p1, long p2, long p3, long p4, long p5, long p6);
}
