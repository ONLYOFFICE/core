using System;
using System.Runtime.Serialization;

namespace CleverAge.OdfConverter.OdfConverterLib
{

    /// <summary>Thrown whenever an error occurs during the build.</summary>
    [Serializable]
    public class NotAnOdfDocumentException : Exception
    {

        /// <summary>Constructs an exception with no descriptive information.</summary>
        public NotAnOdfDocumentException()
            : base()
        {
        }

        /// <summary>Constructs an exception with a descriptive message.</summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        public NotAnOdfDocumentException(String message)
            : base(message)
        {
        }

        /// <summary>Constructs an exception with a descriptive message and a reference to the instance of the <c>Exception</c> that is the root cause of the this exception.</summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">An instance of <c>Exception</c> that is the cause of the current Exception. If <paramref name="innerException"/> is non-null, then the current Exception is raised in a catch block handling <paramref>innerException</paramref>.</param>
        public NotAnOdfDocumentException(String message, Exception innerException)
            : base(message, innerException)
        {
        }

        /// <summary>Initializes a new instance of the exception class with serialized data.</summary>
        /// <param name="info">The object that holds the serialized object data.</param>
        /// <param name="context">The contextual information about the source or destination.</param>
        public NotAnOdfDocumentException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }
    }
}
