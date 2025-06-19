#ifndef TEST_FUNCTIONS_H_
#define TEST_FUNCTIONS_H_


/**
 * NOTE: V8 ONLY!
 * The function tests the work of two CJSContexts in one thread.
 * Current working context is managed with Enter() and Exit() functions, or with CJSContextScope.
 */
void testMultipleContexts();

/**
 * The function tests external embedding functionality by embedding CTestEmbed class.
 */
void testEmbedExternal();

/**
 * NOTE: V8 ONLY!
 * The function tests internal embedding functionality by using CZipEmbed class.
 * It also shows how embedding works for two CJSContext in the same thread (similar to testMultipleContexts()).
 */
void testEmbedInternal();

/**
 * The function tests CHashEmbed class that is embedded internally.
 */
void testHashEmbed();

/**
 * The function tests both internal and external embedding in a more complicated way.
 */
void testEmbedMixed();

#endif // TEST_FUNCTIONS_H_
