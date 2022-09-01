I Этап

1. Собираются шейпы с векторной графикой (DrawPath -> m_arShapes)
  - Копируются текущие Pen и Brush, 
  - определяется тип шейпа VectorTexture или VectorGraphics
  - первоначально определяем положение на стронице (перед текстом/позади текста - пока плохо работает, когда удалялись белые прямоугольники было лучше)
  - задаются геометрические параметры
  -	определяется тип графики (Rectangle, Curve, ComplicatedFigure, NoGraphics) и подтип (LongDash, Dash, Dot, Wave)
  
2. Собираются шейпы-картинки (WriteImage -> m_arImages)
  - задаются геометрические параметры и тип шейпа Picture
  
3. Собираются буквы (CollectTextData -> m_arSymbol)
  - отбрасываем все пробелы (нужно будет добавить доп юникоды для других типов пробелов)
  - работа FontManager
  - задаются геометрические параметры
  - генерим или проверяем на наличие стиль (коприуются и анализируются текущие Font, Brush, PickFontName, PickFontStyle)
  
II Этап 
   Собрали все объекты для текущей страницы. Начинаем анализ.
   
1. Анализируем графику - AnalyzeCollectedShapes() 
  - DetermineLinesType() - превращаем шейпы в горизонтальные линии в зависимости от геометрии, удаляем обработанные шейпы, определяем тип полученной линии на основании типа графики (Rectangle, Curve, ComplicatedFigure, NoGraphics) и подтипа (LongDash, Dash, Dot, Wave). (2 вложенных цикла m_arShapes - m_arShapes с сортировкой вектора)

2. AnalyzeCollectedSymbols() - добавляем свойства каждому символу отдельно
  - Определяем взаимотношения между символами - FontEffects, VertAlignTypeBetweenConts, IsDuplicate (2 вложенных цикла m_arSymbol - m_arSymbol) (удаляем отработанные символы)
  - DetermineStrikeoutsUnderlinesHighlights() - определяем взаимоотношения между графикой и символами Strikeouts, Underlines, Highlights, FontEffect (2 вложенных цикла m_arShapes - m_arSymbol) (удаляем отработанные шейпы)
 
III Этап 
    AnalyzeLines() - Все свойства получены, можно собирать линии

1. BuildLines() - сборка линий, создаются новый линии со своими векторами символов/слов/предложений, (1 цикл m_arSymbol), 
   - собирается вектор m_arTextLine
   - задаются геометрические параметры линии
   - определяется отношение VertAlign между линиями
   - определяется количество линий-дубликатов
2. Сортировка линий по высоте
3. Сшиваются линии с VertAlign (1 цикл m_arTextLine), 
4. CalculateWidth () - Высчитываем ширину линии (2 вложенных цикла m_arTextLine - m_arConts)
5. MergeConts() - Сшиваем буквы/слова/предложения в строке с одним стилем (2 вложенных цикла m_arTextLine - m_arConts)
6. DetermineDominantGraphics() - нужно, чтобы выделить шейп, который будет использоваться в качестве шейдинга параграфа (2 вложенных цикла m_arTextLine - m_arConts)
7. DeleteTextClipPage() - удаление линий вне страницы (1 цикл m_arTextLine)

IV Этап BuildByTypePlainParagraph() - Распределение линий по параграфам и шейпам

V Этап ToXml
