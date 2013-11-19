using System;
using System.Collections.Generic;
using System.Text;

namespace OdfConverter.Transforms
{
    public class PointCoordinates
    {
        private double _x, _y, _z;

        public double X
        {
            get
            {
                return _x;
            }
        }
        public double Y
        {
            get
            {
                return _y;
            }
        }
        public double Z
        {
            get
            {
                return _z;
            }
        }
        public PointCoordinates(double x, double y)
        {
            _x = x;
            _y = y;
            _z = 1;
        }
        public PointCoordinates(double x, double y, double z)
        {
            _x = x;
            _y = y;
            _z = z;
        }
    }

    public class TransformationMatrix
    {
        double _v11, _v12, _v13;
        double _v21, _v22, _v23;
        double _v31, _v32, _v33;
        public TransformationMatrix(double v11, double v12, double v13,
                                    double v21, double v22, double v23,
                                    double v31, double v32, double v33)
        {
            _v11 = v11;
            _v12 = v12;
            _v13 = v13;
            _v21 = v21;
            _v22 = v22;
            _v23 = v23;
            _v31 = v31;
            _v32 = v32;
            _v33 = v33;
        }

        public static TransformationMatrix Identity
        {
            get
            {
                return new TransformationMatrix(1.0, 0.0, 0.0,
                                                0.0, 1.0, 0.0,
                                                0.0, 0.0, 1.0);
            }
        }

        public PointCoordinates Transform(PointCoordinates pt)
        {
            return new PointCoordinates(_v11 * pt.X + _v12 * pt.Y + _v13 * pt.Z,
                                        _v21 * pt.X + _v22 * pt.Y + _v23 * pt.Z,
                                        _v31 * pt.X + _v32 * pt.Y + _v33 * pt.Z);
        }

        //                       _v11, _v12, _v13
        //                       _v21, _v22, _v23
        //                       _v31, _v32, _v33
        // _v11, _v12, _v13
        // _v21, _v22, _v23
        // _v31, _v32, _v33

        public static TransformationMatrix operator *(TransformationMatrix m1, TransformationMatrix m2)
        {
            return new TransformationMatrix(
                m1._v11 * m2._v11 + m1._v12 * m2._v21 + m1._v13 * m2._v31,
                m1._v11 * m2._v12 + m1._v12 * m2._v22 + m1._v13 * m2._v32,
                m1._v11 * m2._v13 + m1._v12 * m2._v23 + m1._v13 * m2._v33,

                m1._v21 * m2._v11 + m1._v22 * m2._v21 + m1._v23 * m2._v31,
                m1._v21 * m2._v12 + m1._v22 * m2._v22 + m1._v23 * m2._v32,
                m1._v21 * m2._v13 + m1._v22 * m2._v23 + m1._v23 * m2._v33,

                m1._v31 * m2._v11 + m1._v32 * m2._v21 + m1._v33 * m2._v31,
                m1._v31 * m2._v12 + m1._v32 * m2._v22 + m1._v33 * m2._v32,
                m1._v31 * m2._v13 + m1._v32 * m2._v23 + m1._v33 * m2._v33);
        }
    }


}