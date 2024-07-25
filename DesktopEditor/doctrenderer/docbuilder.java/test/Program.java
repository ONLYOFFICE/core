import docbuilder.*;

public class Program {
    public static void main(String[] args) {
        String workDirectory = "C:/Program Files/ONLYOFFICE/DocumentBuilder";
        String resultPath = "result.docx";

        docbuilder.utils.Utils.addToPath(workDirectory);

        test(workDirectory, resultPath);

        // Need to explicitly call System.gc() because finalizers might not automatically get called
        // Note: Even System.gc() can not guarantee that finalizers will be actually called. Possible memory leaks!
        System.gc();
    }

    public static void test(String workDirectory, String resultPath) {
        CDocBuilder.initialize(workDirectory);
        CDocBuilder builder = new CDocBuilder();
        builder.createFile(FileTypes.Document.DOCX);

        CDocBuilderContext context = builder.getContext();
        CDocBuilderContextScope scope = context.createScope();

        CDocBuilderValue global = context.getGlobal();

        CDocBuilderValue api = global.get("Api");
        CDocBuilderValue document = api.call("GetDocument");
        CDocBuilderValue paragraph1 = api.call("CreateParagraph");

        // TODO: autowrapping?
        paragraph1.call("SetSpacingAfter", new CDocBuilderValue(1000), new CDocBuilderValue(false));
        paragraph1.call("AddText", new CDocBuilderValue("Hello from Java!"));

        CDocBuilderValue paragraph2 = api.call("CreateParagraph");
        paragraph2.call("AddText", new CDocBuilderValue("Goodbye!"));

        CDocBuilderValue content = context.createArray(2);
        content.set(0, paragraph1);
        content.set(1, paragraph2);

        document.call("InsertContent", content);

        builder.saveFile(FileTypes.Document.DOCX, resultPath);
        builder.closeFile();

        CDocBuilder.dispose();
    }
}
