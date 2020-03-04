package app.x2t.utils

import kotlinx.coroutines.*
import kotlin.coroutines.CoroutineContext

class AsyncRoutines(backgroundContext: CoroutineContext = Dispatchers.Default) {

    protected var mBackgroundContext: CoroutineContext = Dispatchers.Default
    protected var mForegroundContext: CoroutineContext = Dispatchers.Main
    protected var mScope: CoroutineScope = MainScope()

    init {
        mBackgroundContext = backgroundContext
    }

    suspend fun <T> async(context: CoroutineContext = Dispatchers.Default, block: suspend () -> T): Deferred<T> {
        return mScope.async(context) {
            block.invoke()
        }
    }

    fun <T> run(onBackground: suspend (foreground: CoroutineScope, instance: AsyncRoutines) -> T, onComplete: ((T) -> Unit)? = null, onError: ((Throwable) -> Unit)? = null): Job {
        return mScope.launch(mForegroundContext) {
            try {
                val result = withContext(mBackgroundContext) {
                    onBackground.invoke(this@launch, this@AsyncRoutines)
                }
                onComplete?.invoke(result)
            } catch (e: Exception) {
                onError?.invoke(e)
            }
        }
    }

    fun cancel(): Boolean {
        return try {
            mScope.cancel()
            true
        } catch (e: CancellationException) {
            false
        } finally {
            mScope = MainScope()
        }
    }
}