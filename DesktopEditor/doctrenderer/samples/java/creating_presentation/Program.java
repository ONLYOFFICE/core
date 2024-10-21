import docbuilder.*;

import java.util.HashMap;

public class Program {
    public static void main(String[] args) {
        String resultPath = "result.pptx";
        // Fill slide images
        HashMap<String, String> slideImages = new HashMap<String, String>();
        slideImages.put("gun", "https://api.onlyoffice.com/content/img/docbuilder/examples/presentation_gun.png");
        slideImages.put("axe", "https://api.onlyoffice.com/content/img/docbuilder/examples/presentation_axe.png");
        slideImages.put("knight", "https://api.onlyoffice.com/content/img/docbuilder/examples/presentation_knight.png");
        slideImages.put("sky", "https://api.onlyoffice.com/content/img/docbuilder/examples/presentation_sky.png");

        createPresentation(resultPath, slideImages);

        // Need to explicitly call System.gc() to free up resources
        System.gc();
    }

    public static void createPresentation(String resultPath, HashMap<String, String> slideImages) {
        int doctype = FileTypes.Presentation.PPTX;

        // Initialize builder with docbuilder.jar directory by passing empty String
        CDocBuilder.initialize("");
        CDocBuilder builder = new CDocBuilder();
        builder.createFile(doctype);

        CDocBuilderContext context = builder.getContext();
        CDocBuilderValue global = context.getGlobal();
        CDocBuilderValue api = global.get("Api");

        // Create presentation
        CDocBuilderValue presentation = api.call("GetPresentation");
        presentation.call("SetSizes", 9144000, 6858000);

        CDocBuilderValue slide = createImageSlide(api, presentation, slideImages.get("gun"));
        presentation.call("GetSlideByIndex", 0).call("Delete");

        CDocBuilderValue shape = api.call("CreateShape", "rect", 8056800, 3020400, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 608400, 1267200);
        CDocBuilderValue content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "How They", 160, true, "left");
        addTextToSlideShape(api, content, "Throw Out", 132, false, "left");
        addTextToSlideShape(api, content, "a Challenge", 132, false, "left");
        slide.call("AddObject", shape);

        slide = createImageSlide(api, presentation, slideImages.get("axe"));

        shape = api.call("CreateShape", "rect", 6904800, 1724400, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 1764000, 1191600);
        content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "American Indians ", 110, true, "right");
        addTextToSlideShape(api, content, "(XVII century)", 94, false, "right");
        slide.call("AddObject", shape);

        shape = api.call("CreateShape", "rect", 4986000, 2419200, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 3834000, 3888000);
        content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "put a tomahawk on the ground in the ", 84, false, "right");
        addTextToSlideShape(api, content, "rival's camp", 84, false, "right");
        slide.call("AddObject", shape);

        slide = createImageSlide(api, presentation, slideImages.get("knight"));

        shape = api.call("CreateShape", "rect", 6904800, 1724400, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 1764000, 1191600);
        content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "European Knights", 110, true, "right");
        addTextToSlideShape(api, content, " (XII-XVI centuries)", 94, false, "right");
        slide.call("AddObject", shape);

        shape = api.call("CreateShape", "rect", 4986000, 2419200, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 3834000, 3888000);
        content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "threw a glove", 84, false, "right");
        addTextToSlideShape(api, content, "in the rival's face", 84, false, "right");
        slide.call("AddObject", shape);

        slide = createImageSlide(api, presentation, slideImages.get("sky"));

        shape = api.call("CreateShape", "rect", 7887600, 3063600, api.call("CreateNoFill"), api.call("CreateStroke", 0, api.call("CreateNoFill")));
        shape.call("SetPosition", 630000, 1357200);
        content = shape.call("GetDocContent");
        content.call("RemoveAllElements");
        addTextToSlideShape(api, content, "OnlyOffice", 176, false, "center");
        addTextToSlideShape(api, content, "stands for Peace", 132, false, "center");
        slide.call("AddObject", shape);

        // Save file and close DocBuilder
        builder.saveFile(doctype, resultPath);
        builder.closeFile();

        CDocBuilder.dispose();
    }

    public static CDocBuilderValue createImageSlide(CDocBuilderValue api, CDocBuilderValue presentation, String imageUrl) {
        CDocBuilderValue slide = api.call("CreateSlide");
        presentation.call("AddSlide", slide);
        CDocBuilderValue fill = api.call("CreateBlipFill", imageUrl, "stretch");
        slide.call("SetBackground", fill);
        slide.call("RemoveAllObjects");
        return slide;
    }

    public static void addTextToSlideShape(CDocBuilderValue api, CDocBuilderValue content, String text, int fontSize, boolean isBold, String js) {
        CDocBuilderValue paragraph = api.call("CreateParagraph");
        paragraph.call("SetSpacingBefore", 0);
        paragraph.call("SetSpacingAfter", 0);
        content.call("Push", paragraph);
        CDocBuilderValue run = paragraph.call("AddText", text);
        run.call("SetFill", api.call("CreateSolidFill", api.call("CreateRGBColor", 0xff, 0xff, 0xff)));
        run.call("SetFontSize", fontSize);
        run.call("SetFontFamily", "Georgia");
        run.call("SetBold", isBold);
        paragraph.call("SetJc", js);
    }
}
