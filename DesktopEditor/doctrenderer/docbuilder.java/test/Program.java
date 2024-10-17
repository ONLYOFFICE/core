import docbuilder.*;

public class Program {
    public static void main(String[] args) {
        String resultPath = "result.docx";

        test(resultPath);

        // Need to explicitly call System.gc() because finalizers might not automatically get called
        // Note: Even System.gc() can not guarantee that finalizers will be actually called. Possible memory leaks!
        System.gc();
    }

    public static void test(String resultPath) {
        CDocBuilder.initialize("");
        CDocBuilder builder = new CDocBuilder();
        builder.createFile(FileTypes.Document.DOCX);

        CDocBuilderContext context = builder.getContext();

        CDocBuilderValue global = context.getGlobal();

        CDocBuilderValue api = global.get("Api");
        CDocBuilderValue document = api.call("GetDocument");
        CDocBuilderValue paragraph1 = api.call("CreateParagraph");

        paragraph1.call("SetSpacingAfter", 1000, false);
        paragraph1.call("AddText", "Hello from Java!");

        CDocBuilderValue paragraph2 = api.call("CreateParagraph");
        paragraph2.call("AddText", "Goodbye!");

        CDocBuilderValue[] paragraphs = { paragraph1, paragraph2 };
        CDocBuilderValue content = new CDocBuilderValue(paragraphs);

        document.call("InsertContent", content);

        builder.saveFile(FileTypes.Document.DOCX, resultPath);
        builder.closeFile();

        CDocBuilder.dispose();
    }
}
