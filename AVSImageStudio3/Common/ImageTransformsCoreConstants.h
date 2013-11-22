#pragma once

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			/* @desc: */
			// ќшибки, возвращ€емые функци€ми трансформа
			const int c_nErrorNone = 0;								// все хорошо - ошибки нет
			const int c_nErrorInvalidPointer = -1;					// указатель на интерфейс или какой-то необходимый входной параметр - NULL
			const int c_nErrorInvalidInterface = -2;				// указатель на интерфейс не NULL, но указывает на интерфейс не того типа, который ожидаетс€
			const int c_nErrorInvalidFormat = -3;					// формат картинки, лежащей в интерфейсе не совпадает с требуемым
			const int c_nErrorNoMemory = -4;						// не хватает пам€ти дл€ того, чтобы произвести какую-то операцию
			const int c_nErrorInvalidSourceImage = -10;				// входна€ картинка невалидна
			const int c_nErrorNotImplemented = -100;				// функциональность еще не реализована
			const int c_nErrorBaseInterfacesNotFound = -200;		// не найдены необходимые интерфейсы (обычно низкоуровневые компоненты)
			const int c_nErrorGdiplusFailure = -300;				// произошла ошибка в gdiplus и результат не соответствует ожидаемому; возможно gdiplus не инициализирован, либо параметры, поданные в gdiplus метод невалидны
			const int c_nErrorPixelNotFound = -400;					// пиксел с заданными координатами находитс€ вне обрабатываемой картинки
			const int c_nErrorPathIsEmpty = -500;					// графический путь (GraphicsPath) не содержит точек
			const int c_nErrorInvalidPointsCount = -510;			// количество точек неправильно
			const int c_nErrorAlphaCopositionFailure = -600;		// внутренн€€ функци€, реализующа€ смешение цветов не смогла завершить преобразование корректно
			const int c_nErrorImageCopositionFailure = -610;		// внутренн€€ функци€, реализующа€ смешение картинок не смогла завершить преобразование корректно
			const int c_nErrorTextPainterFailure = -700;			// произошла ошибка в text painter'е при вызове функции DrawText (см. ImagePaintText.h) возможно инициализаци€ text painter'а неполна€ (т.е. вызываема€ анимаци€ не подсчитана корректно)
			/* @desc */

			/* @desc: */
			// “ипы титров (двигающегос€ текста), которые поддерживаютс€ функцией GDIPLUS_DrawCredits
			const int c_nCreditsHMMBottom	= 5100;
			const int c_nCreditsHMMCenter	= 5101;
			const int c_nCreditsHMMTop		= 5102;
			const int c_nCreditsHMWBottom	= 5200;
			const int c_nCreditsHMWCenter	= 5201;
			const int c_nCreditsHMWTop		= 5202;
			const int c_nCreditsHMSBottom	= 5300;
			const int c_nCreditsHMSCenter	= 5301;
			const int c_nCreditsHMSTop		= 5302;
			const int c_nCreditsHMHBottom	= 5400;
			const int c_nCreditsHMHCenter	= 5401;
			const int c_nCreditsHMHTop		= 5402;
			const int c_nCreditsHHMBottom	= 5500;
			const int c_nCreditsHHMCenter	= 5501;
			const int c_nCreditsHHMTop		= 5502;

			const int c_nCreditsVMMLeft		= 6100;
			const int c_nCreditsVMMCenter	= 6101;
			const int c_nCreditsVMMRight	= 6102;
			const int c_nCreditsVMWLeft		= 6200;
			const int c_nCreditsVMWCenter	= 6201;
			const int c_nCreditsVMWRight	= 6202;
			const int c_nCreditsVMSLeft		= 6300;
			const int c_nCreditsVMSCenter	= 6301;
			const int c_nCreditsVMSRight	= 6302;
			const int c_nCreditsVMHLeft		= 6400;
			const int c_nCreditsVMHCenter	= 6401;
			const int c_nCreditsVMHRight	= 6402;
			const int c_nCreditsVHMLeft		= 6500;
			const int c_nCreditsVHMCenter	= 6501;
			const int c_nCreditsVHMRight	= 6502;
			/* @desc */

			enum EImageFileType
			{
				c_nImageFileTypeAny    = 0,
				c_nImageFileTypeBmp    = 1,
				c_nImageFileTypeJpg    = 2,
				c_nImageFileTypeGif    = 3,
				c_nImageFileTypeTiff   = 4,
				c_nImageFileTypePsd    = 5,
				c_nImageFileTypeTga    = 6,
				c_nImageFileTypeRas    = 7,
				c_nImageFileTypePcx    = 8,
				c_nImageFileTypeIPod   = 9,
				c_nImageFileTypeRaw    = 10,
				c_nImageFileTypeJp2    = 11,
			};
		}
	}
}