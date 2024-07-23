import docbuilder.CDocBuilder;

public class Program {
    public static void main(String[] args) {
        test();

        // Need to explicitly call System.gc() because finalizers might not automatically get called
        // Note: Even System.gc() can not guarantee that finalizers will be actually called. Possible memory leaks!
        System.gc();
    }

    public static void test() {
        CDocBuilder.initialize("C:\\Users\\mixa_\\main\\core\\build\\lib\\win_64\\debug");
        CDocBuilder builder = new CDocBuilder();
        System.out.println(builder.getVersion());
        CDocBuilder.dispose();
    }
}
