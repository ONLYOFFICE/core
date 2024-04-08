Логика описана в CPage::ProcessingAndRecordingOfPageData

I Этап

1. Собираются шейпы с векторной графикой (DrawPath -> m_arShapes)
  - Копируются текущие Pen и Brush, 
  - определяется тип шейпа VectorTexture или VectorGraphics
  - первоначально определяем положение на стронице (перед текстом/позади текста - пока плохо работает, когда удалялись белые прямоугольники было лучше)
  - задаются геометрические параметры
  - определяется тип графики (Rectangle, Curve, ComplicatedFigure, NoGraphics) и подтип (LongDash, Dash, Dot, Wave)
  
2. Собираются шейпы-картинки (WriteImage -> m_arImages)
  - задаются геометрические параметры и тип шейпа Picture
  
3. Собираются буквы и сразу распределются по текстовым линиям. Отдельно собираются DiacriticalSymbol (CollectTextData -> m_arTextLine, m_arDiacriticalSymbol)
  - отбрасываем все пробелы (нужно будет добавить доп юникоды для других типов пробелов)
  - работа FontManager
  - задаются геометрические параметры
  - генерим или проверяем на наличие стиль (копируются и анализируются текущие Font, Brush, PickFontName, PickFontStyle)
  
II Этап 
   Собрали все объекты для текущей страницы. Начинаем анализ.
   
1. Анализируем графику - AnalyzeCollectedShapes()
  - BuildTables(); - собираем таблицы из шейпов (в разработке)
  - DetermineLinesType() - превращаем шейпы в горизонтальные линии в зависимости от геометрии, удаляем обработанные шейпы, определяем тип полученной линии на основании типа графики (Rectangle, Curve, ComplicatedFigure, NoGraphics) и подтипа (LongDash, Dash, Dot, Wave). (2 вложенных цикла m_arShapes - m_arShapes с сортировкой вектора)

2. AnalyzeCollectedTextLines() - добавляем свойства каждому символу отдельно
  - Определяем взаимотношения между символами - FontEffects, VertAlignTypeBetweenConts, IsDuplicate (2 вложенных цикла m_arSymbol - m_arSymbol) (удаляем отработанные символы)
  - DetermineStrikeoutsUnderlinesHighlights() - определяем взаимоотношения между графикой и символами Strikeouts, Underlines, Highlights, FontEffect (2 вложенных цикла m_arShapes - m_arSymbol) (удаляем отработанные шейпы)
  - AddDiacriticalSymbols() - добавляем DiacriticalSymbol
  - MergeLinesByVertAlignType() - объединяем линии с определенными eVertAlignType
  - DeleteTextClipPage() - удаление линий вне страницы (1 цикл m_arTextLine)
  - DetermineTextColumns() - определяем колонки текста и добавляем в таблицу (в разработке)
  - BuildLines() - собираем из символов слова, добавляем пробелы
  - DetermineDominantGraphics() - нужно, чтобы выделить шейп, который будет использоваться в качестве шейдинга параграфа (2 вложенных цикла m_arTextLine - m_arConts)
  - BuildParagraphes() - собираем из текстовых строк параграфы/шейпы и добавляем в m_arOutputObjects

III Этап ToXml
