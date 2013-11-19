using System;
using OdfConverter.Transforms;

namespace OdfConverter.Transforms.Test
{
    public class OoxShape
    {
        // unused
        //static double Scale = 10.0;
        //static int DeltaX = 0;
        //static int DeltaY = 0;


        OoxTransform _transform;

        public OoxTransform OoxTransform
        {
            get
            {
                return _transform;
            }
        }

        public OoxShape(OoxTransform transform)
        {
            _transform = transform;

        }

    }
}