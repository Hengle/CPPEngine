/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef __Shape3D__
#define __Shape3D__
#include <FW3.h>

	enum tShape
	{
		eTexturedBox,
		eBox,
		eSphere,
		eCylinder
	};

    /// <summary>
    /// Defines a custom vertex format structure that contains position and normal data.
    /// </summary>
    struct VertexPositionNormal
    {
        /// <summary>
        /// The vertex position.
        /// </summary>
        D3DXFROMWINEVECTOR3 Position;

        /// <summary>
        /// The vertex normal.
        /// </summary>
        D3DXFROMWINEVECTOR3 Normal;

        /// <summary>
        /// Initializes a new instance of the VertexPositionNormal class.
        /// </summary>
        /// <param name="position">The position of the vertex.</param>
        /// <param name="normal">The vertex normal.</param>
        VertexPositionNormal()
        {
            Position = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f);
            Normal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f);
        }
	VertexPositionNormal(D3DXFROMWINEVECTOR3 position, D3DXFROMWINEVECTOR3 normal)
        {
            Position = position;
            Normal = normal;
        }

        /// <summary>
        /// Gets the size of the VertexPositionNormal class.
        /// </summary>
	#ifndef SWIG
        const static unsigned int VertexSizeInBytes = sizeof(D3DXFROMWINEVECTOR3) * 2;
	#endif
    };
    struct VertexPositionNormalTexture
    {
        /// <summary>
        /// The vertex position.
        /// </summary>
        D3DXFROMWINEVECTOR3 Position;

        /// <summary>
        /// The vertex normal.
        /// </summary>
        D3DXFROMWINEVECTOR3 Normal;
        D3DXFROMWINEVECTOR2 Tex;

        /// <summary>
        /// Initializes a new instance of the VertexPositionNormal class.
        /// </summary>
        /// <param name="position">The position of the vertex.</param>
        /// <param name="normal">The vertex normal.</param>
        VertexPositionNormalTexture()
        {
            Position = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f);
            Normal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 0.0f);
		Tex = D3DXFROMWINEVECTOR2(0.0f, 0.0f);
        }
	VertexPositionNormalTexture(D3DXFROMWINEVECTOR3 position, D3DXFROMWINEVECTOR3 normal, D3DXFROMWINEVECTOR2 tex)
        {
            Position = position;
            Normal = normal;
		Tex = tex;
        }

        /// <summary>
        /// Gets the size of the VertexPositionNormal class.
        /// </summary>
	#ifndef SWIG
        const static unsigned int VertexSizeInBytes = sizeof(D3DXFROMWINEVECTOR3)*2 + sizeof(D3DXFROMWINEVECTOR2);
	#endif
    };

    /// <summary>
    /// Defines a simple 3D shape, such as a box, sphere, or cylinder.
    /// </summary>
    class Shape3D
    {
	protected:
        unsigned int VertexCount;
        unsigned int TriangleCount;
	TextureID texID;
	ShaderID Shader;
        VertexBufferID VertexBuffer;
        IndexBufferID IndexBuffer;
        VertexFormatID VertexDeclaration;
	unsigned int VertexSizeInBytes;
	public:
	Shape3D(Shape3D& rhs)
	{
		VertexCount=rhs.VertexCount;
        	TriangleCount=rhs.TriangleCount;
		texID=rhs.texID;
		Shader=rhs.Shader;
        	VertexBuffer=rhs.VertexBuffer;
        	IndexBuffer=rhs.IndexBuffer;
        	VertexDeclaration=rhs.VertexDeclaration;
		VertexSizeInBytes=rhs.VertexSizeInBytes;
	}
        /// <summary>
        /// The number of vertices making up the shape.
        /// </summary>
	unsigned int getVertexCount();
	void setVertexCount(unsigned int aVertexCount);

        /// <summary>
        /// The number of triangle making up the shape.
        /// </summary>
	unsigned int getTriangleCount();
	void setTriangleCount(unsigned int aTriangleCount);

        /// <summary>
        /// The buffer of vertices making up the shape.
        /// </summary>
	VertexBufferID getVertexBuffer();
	void setVertexBuffer(VertexBufferID aVertexBuffer);

        /// <summary>
        /// The buffer of indices making up the shape, if it has one.
        /// </summary>
	IndexBufferID getIndexBuffer();
	void setIndexBuffer(IndexBufferID aIndexBuffer);

        /// <summary>
        /// The declaration of the type of vertices making up the shape.
        /// </summary>
	VertexFormatID getVertexDeclaration();
	void setVertexDeclaration(VertexFormatID aVertexDeclaration);

        /// <summary>
        /// The byte size of each vertex making up the shape.
        /// </summary>

	
	unsigned int getVertexSizeInBytes(){return VertexSizeInBytes;}
	void setVertexSizeInBytes(unsigned int aVertexSizeInBytes){VertexSizeInBytes=aVertexSizeInBytes;}

        /// <summary>
        /// Initializes a new instance of a <see cref="Shape3D"/>.
        /// Private so that a shape can only be instantiated from the Create methods.
        /// </summary>
        Shape3D();

        /// <summary>
        /// Draws the shape.
        /// </summary>
        /// <param name="device">The device to use to draw the shape.</param>
        void Draw(D3DXFROMWINEMATRIX& mat);

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormalTexture"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void CreateTexturedBox(float width, float height, float depth, const char* aShaderName);

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormal"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void CreateBox(float width, float height, float depth, const char* aShaderName);

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormal"/> vertices forming a sphere.
        /// </summary>
        /// <remarks>
        /// Draw with an INDEXED TriangleList.  The values to use are as follows:
        /// Vertex Count    = slices * (stacks - 1) + 2
        /// Primitive Count = slices * (stacks - 1) * 2
        /// </remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created sphere.</param>
        /// <param name="radius">Radius of the sphere. This value should be greater than or equal to 0.0f.</param>
        /// <param name="slices">Number of slices about the Y axis.</param>
        /// <param name="stacks">Number of stacks along the Y axis. Should be 2 or greater. (stack of 1 is just a cylinder)</param>
        void CreateSphere(float radius, unsigned int slices, unsigned int stacks, const char* aShaderName);

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormal"/> vertices forming a cylinder.
        /// </summary>
        /// <remarks>
        /// Draw with an INDEXED TriangleList.  The values to use are as follows:
        /// Vertex Count    = slices * (stacks + 1) + 2
        /// Primitive Count = slices * (stacks + 1) * 2
        /// </remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created cylinder.</param>
        /// <param name="bottomRadius">Radius at the negative Y end. Value should be greater than or equal to 0.0f.</param>
        /// <param name="topRadius">Radius at the positive Y end. Value should be greater than or equal to 0.0f.</param>
        /// <param name="length">Length of the cylinder along the Y-axis.</param>
        /// <param name="slices">Number of slices about the Y axis.</param>
        /// <param name="stacks">Number of stacks along the Y axis.</param>
        void CreateCylinder(float bottomRadius, float topRadius, float length, unsigned int slices, unsigned int stacks, const char* aShaderName);

        /// <summary>
        /// Creates an <see cref="IndexBuffer"/> for spherical shapes like Spheres, Cylinders, and Cones.
        /// </summary>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created cylinder.</param>
        /// <param name="vertexCount">The total number of vertices making up the shape.</param>
        /// <param name="indexCount">The total number of indices making up the shape.</param>
        /// <param name="slices">The number of slices about the Y axis.</param>
        /// <returns>The index buffer containing the index data for the shape.</returns>
        IndexBufferID CreateIndexBuffer(unsigned int vertexCount, unsigned int indexCount, unsigned int slices);
	};

struct FW3ShadersFactory
{
protected:
    	static std::map<std::string, ShaderID> m_FW3ShadersFactory_Shadercache;
	static ShaderID find_from_FW3ShadersFactory_Shadercache(const char* aShaderName);
	static void insert_to_FW3ShadersFactory_Shadercache(const char* aShaderName, ShaderID id);
    	static std::map<tShape, VertexFormatID> m_FW3ShadersFactory_VertexFormatcache;
	static VertexFormatID find_from_FW3ShadersFactory_VertexFormatcache(tShape aShape);
	static void insert_to_FW3ShadersFactory_VertexFormatcache(tShape aShape, VertexFormatID id);
public:
	static ShaderID GetShader(const char* aShaderName, const char *vsName, const char *psName);
	static VertexFormatID GetVertexFormat(tShape aShape, const ShaderID shd);
	static const char* GetShaderTxt(const char* aShaderName);
};
#endif

