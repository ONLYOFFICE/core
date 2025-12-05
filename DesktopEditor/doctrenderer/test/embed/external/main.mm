#include "test_functions.h"

int main()
{
	// all test functions should be called inside the `@autoreleasepool` block!
	@autoreleasepool
	{
		testEmbedExternal();
		// testHashEmbed();
		// testEmbedMixed();
	}

	return 0;
}
