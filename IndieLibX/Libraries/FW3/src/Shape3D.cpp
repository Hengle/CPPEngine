/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include "Shape3D.h"





	unsigned int Shape3D::getVertexCount(){return VertexCount;}
	void Shape3D::setVertexCount(unsigned int aVertexCount){VertexCount=aVertexCount;}

	unsigned int Shape3D::getTriangleCount(){return TriangleCount;}
	void Shape3D::setTriangleCount(unsigned int aTriangleCount){TriangleCount=aTriangleCount;}

	VertexBufferID Shape3D::getVertexBuffer(){return VertexBuffer;}
	void Shape3D::setVertexBuffer(VertexBufferID aVertexBuffer){VertexBuffer=aVertexBuffer;}

	IndexBufferID Shape3D::getIndexBuffer(){return IndexBuffer;}
	void Shape3D::setIndexBuffer(IndexBufferID aIndexBuffer){IndexBuffer=aIndexBuffer;}

	VertexFormatID Shape3D::getVertexDeclaration(){return VertexDeclaration;}
	void Shape3D::setVertexDeclaration(VertexFormatID aVertexDeclaration){VertexDeclaration=aVertexDeclaration;}

        /// <summary>
        /// The byte size of each vertex making up the 
        /// </summary>

        /// <summary>
        /// Initializes a new instance of a <see cref="Shape3D"/>.
        /// Private so that a shape can only be instantiated from the Create methods.
        /// </summary>
        Shape3D::Shape3D()
        {
		texID = -1;
            VertexCount = 0;
            TriangleCount = 0;
            VertexSizeInBytes = 0;
            VertexBuffer = -1;
            IndexBuffer = -1;
            VertexDeclaration = -1;
        }

        /// <summary>
        /// Draws the 
        /// </summary>
        /// <param name="device">The device to use to draw the </param>
        void Shape3D::Draw(D3DXFROMWINEMATRIX& mat)
        {
		if(VertexDeclaration==-1)
			return;
		if(VertexBuffer==-1)
			return;

           IRenderer::GetRendererInstance()->setShader(Shader);

	IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProjection", mat);
 
            IRenderer::GetRendererInstance()->setVertexFormat(VertexDeclaration);

		if(texID!=-1) IRenderer::GetRendererInstance()->setTexture("base", texID);

            IRenderer::GetRendererInstance()->setVertexBuffer(0, VertexBuffer);

            if (IndexBuffer != -1)
		{
            	IRenderer::GetRendererInstance()->setIndexBuffer(IndexBuffer);
                IRenderer::GetRendererInstance()->DrawIndexedPrimitive(PRIM_TRIANGLES, 0, 0, VertexCount, 0, TriangleCount);
		}
	  else
          	IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLES, 0, TriangleCount);
		
#if 0
		LOG_FNLN;
		LOG_PRINT("Shader=%x\n", Shader);
		LOG_PRINT("vf=%x\n", VertexDeclaration);
		LOG_PRINT("texID=%x\n", texID);
		LOG_PRINT("VertexBuffer=%x\n", VertexBuffer);
		LOG_PRINT("IndexBuffer=%x\n", IndexBuffer);
		LOG_PRINT("VertexCount=%x\n", VertexCount);
		LOG_PRINT("TriangleCount=%x\n", TriangleCount);
#endif
        }

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormalTexture"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void Shape3D::CreateTexturedBox(float width, float height, float depth, const char* aShaderName)
        {
            

            // Because the box is centered at the origin, need to divide by two to find the + and - offsets
            width = width / 2.0f;
            height = height / 2.0f;
            depth = depth / 2.0f;

            VertexPositionNormalTexture cubeVertices[36];

            D3DXFROMWINEVECTOR3 topLeftFront = D3DXFROMWINEVECTOR3(-width, height, depth);
            D3DXFROMWINEVECTOR3 bottomLeftFront = D3DXFROMWINEVECTOR3(-width, -height, depth);
            D3DXFROMWINEVECTOR3 topRightFront = D3DXFROMWINEVECTOR3(width, height, depth);
            D3DXFROMWINEVECTOR3 bottomRightFront = D3DXFROMWINEVECTOR3(width, -height, depth);
            D3DXFROMWINEVECTOR3 topLeftBack = D3DXFROMWINEVECTOR3(-width, height, -depth);
            D3DXFROMWINEVECTOR3 topRightBack = D3DXFROMWINEVECTOR3(width, height, -depth);
            D3DXFROMWINEVECTOR3 bottomLeftBack = D3DXFROMWINEVECTOR3(-width, -height, -depth);
            D3DXFROMWINEVECTOR3 bottomRightBack = D3DXFROMWINEVECTOR3(width, -height, -depth);

            D3DXFROMWINEVECTOR2 textureTopLeft = D3DXFROMWINEVECTOR2(0.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureTopRight = D3DXFROMWINEVECTOR2(1.0f, 0.0f);
            D3DXFROMWINEVECTOR2 textureBottomLeft = D3DXFROMWINEVECTOR2(0.0f, 1.0f);
            D3DXFROMWINEVECTOR2 textureBottomRight = D3DXFROMWINEVECTOR2(1.0f, 1.0f);

            D3DXFROMWINEVECTOR3 frontNormal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 1.0f);
            D3DXFROMWINEVECTOR3 backNormal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, -1.0f);
            D3DXFROMWINEVECTOR3 topNormal = D3DXFROMWINEVECTOR3(0.0f, 1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 bottomNormal = D3DXFROMWINEVECTOR3(0.0f, -1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 leftNormal = D3DXFROMWINEVECTOR3(-1.0f, 0.0f, 0.0f);
            D3DXFROMWINEVECTOR3 rightNormal = D3DXFROMWINEVECTOR3(1.0f, 0.0f, 0.0f);

            // Front face.
            cubeVertices[1] = VertexPositionNormalTexture(topLeftFront, frontNormal, textureTopLeft);
            cubeVertices[0] = VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomLeft);
            cubeVertices[2] = VertexPositionNormalTexture(topRightFront, frontNormal, textureTopRight);
            cubeVertices[4] = VertexPositionNormalTexture(bottomLeftFront, frontNormal, textureBottomLeft);
            cubeVertices[3] = VertexPositionNormalTexture(bottomRightFront, frontNormal, textureBottomRight);
            cubeVertices[5] = VertexPositionNormalTexture(topRightFront, frontNormal, textureTopRight);

            // Back face.
            cubeVertices[7] = VertexPositionNormalTexture(topLeftBack, backNormal, textureTopRight);
            cubeVertices[6] = VertexPositionNormalTexture(topRightBack, backNormal, textureTopLeft);
            cubeVertices[8] = VertexPositionNormalTexture(bottomLeftBack, backNormal, textureBottomRight);
            cubeVertices[10] = VertexPositionNormalTexture(bottomLeftBack, backNormal, textureBottomRight);
            cubeVertices[9] = VertexPositionNormalTexture(topRightBack, backNormal, textureTopLeft);
            cubeVertices[11] = VertexPositionNormalTexture(bottomRightBack, backNormal, textureBottomLeft);

            // Top face.
            cubeVertices[13] = VertexPositionNormalTexture(topLeftFront, topNormal, textureBottomLeft);
            cubeVertices[12] = VertexPositionNormalTexture(topRightBack, topNormal, textureTopRight);
            cubeVertices[14] = VertexPositionNormalTexture(topLeftBack, topNormal, textureTopLeft);
            cubeVertices[16] = VertexPositionNormalTexture(topLeftFront, topNormal, textureBottomLeft);
            cubeVertices[15] = VertexPositionNormalTexture(topRightFront, topNormal, textureBottomRight);
            cubeVertices[17] = VertexPositionNormalTexture(topRightBack, topNormal, textureTopRight);

            // Bottom face.
            cubeVertices[19] = VertexPositionNormalTexture(bottomLeftFront, bottomNormal, textureTopLeft);
            cubeVertices[18] = VertexPositionNormalTexture(bottomLeftBack, bottomNormal, textureBottomLeft);
            cubeVertices[20] = VertexPositionNormalTexture(bottomRightBack, bottomNormal, textureBottomRight);
            cubeVertices[22] = VertexPositionNormalTexture(bottomLeftFront, bottomNormal, textureTopLeft);
            cubeVertices[21] = VertexPositionNormalTexture(bottomRightBack, bottomNormal, textureBottomRight);
            cubeVertices[23] = VertexPositionNormalTexture(bottomRightFront, bottomNormal, textureTopRight);

            // Left face.
            cubeVertices[25] = VertexPositionNormalTexture(topLeftFront, leftNormal, textureTopRight);
            cubeVertices[24] = VertexPositionNormalTexture(bottomLeftBack, leftNormal, textureBottomLeft);
            cubeVertices[26] = VertexPositionNormalTexture(bottomLeftFront, leftNormal, textureBottomRight);
            cubeVertices[28] = VertexPositionNormalTexture(topLeftBack, leftNormal, textureTopLeft);
            cubeVertices[27] = VertexPositionNormalTexture(bottomLeftBack, leftNormal, textureBottomLeft);
            cubeVertices[29] = VertexPositionNormalTexture(topLeftFront, leftNormal, textureTopRight);

            // Right face.
            cubeVertices[31] = VertexPositionNormalTexture(topRightFront, rightNormal, textureTopLeft);
            cubeVertices[30] = VertexPositionNormalTexture(bottomRightFront, rightNormal, textureBottomLeft);
            cubeVertices[32] = VertexPositionNormalTexture(bottomRightBack, rightNormal, textureBottomRight);
            cubeVertices[34] = VertexPositionNormalTexture(topRightBack, rightNormal, textureTopRight);
            cubeVertices[33] = VertexPositionNormalTexture(topRightFront, rightNormal, textureTopLeft);
            cubeVertices[35] = VertexPositionNormalTexture(bottomRightBack, rightNormal, textureBottomRight);

            // Create the actual vertex buffer
	    VertexBuffer=IRenderer::GetRendererInstance()->addVertexBuffer(VertexPositionNormalTexture::VertexSizeInBytes * 36, STATIC, cubeVertices);

            VertexCount = 36;
            TriangleCount = 12;

	Shader = FW3ShadersFactory::GetShader(aShaderName, "main", "main");
	VertexDeclaration = FW3ShadersFactory::GetVertexFormat(eTexturedBox, Shader);

            VertexSizeInBytes = VertexPositionNormalTexture::VertexSizeInBytes;

            SamplerStateID ss=IRenderer::GetRendererInstance()->Getlinear();
		texID=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, ss);
		
        }

        /// <summary>
        /// Creates a <see cref="VertexBuffer"/> filled with <see cref="VertexPositionNormal"/> vertices forming a box.
        /// </summary>
        /// <remarks>Always draw using a non-indexed TriangleList of 12 primitives.</remarks>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created box.</param>
        /// <param name="width">Width of the box, along the x-axis.</param>
        /// <param name="height">Height of the box, along the y-axis.</param>
        /// <param name="depth">Depth of the box, along the z-axis.</param>
        void Shape3D::CreateBox(float width, float height, float depth, const char* aShaderName)
        {
            

            // Because the box is centered at the origin, need to divide by two to find the + and - offsets
            width = width / 2.0f;
            height = height / 2.0f;
            depth = depth / 2.0f;

            VertexPositionNormal cubeVertices[36];

            D3DXFROMWINEVECTOR3 topLeftFront = D3DXFROMWINEVECTOR3(-width, height, depth);
            D3DXFROMWINEVECTOR3 bottomLeftFront = D3DXFROMWINEVECTOR3(-width, -height, depth);
            D3DXFROMWINEVECTOR3 topRightFront = D3DXFROMWINEVECTOR3(width, height, depth);
            D3DXFROMWINEVECTOR3 bottomRightFront = D3DXFROMWINEVECTOR3(width, -height, depth);
            D3DXFROMWINEVECTOR3 topLeftBack = D3DXFROMWINEVECTOR3(-width, height, -depth);
            D3DXFROMWINEVECTOR3 topRightBack = D3DXFROMWINEVECTOR3(width, height, -depth);
            D3DXFROMWINEVECTOR3 bottomLeftBack = D3DXFROMWINEVECTOR3(-width, -height, -depth);
            D3DXFROMWINEVECTOR3 bottomRightBack = D3DXFROMWINEVECTOR3(width, -height, -depth);

            D3DXFROMWINEVECTOR3 frontNormal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, 1.0f);
            D3DXFROMWINEVECTOR3 backNormal = D3DXFROMWINEVECTOR3(0.0f, 0.0f, -1.0f);
            D3DXFROMWINEVECTOR3 topNormal = D3DXFROMWINEVECTOR3(0.0f, 1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 bottomNormal = D3DXFROMWINEVECTOR3(0.0f, -1.0f, 0.0f);
            D3DXFROMWINEVECTOR3 leftNormal = D3DXFROMWINEVECTOR3(-1.0f, 0.0f, 0.0f);
            D3DXFROMWINEVECTOR3 rightNormal = D3DXFROMWINEVECTOR3(1.0f, 0.0f, 0.0f);

            // Front face.
            cubeVertices[1] = VertexPositionNormal(topLeftFront, frontNormal);
            cubeVertices[0] = VertexPositionNormal(bottomLeftFront, frontNormal);
            cubeVertices[2] = VertexPositionNormal(topRightFront, frontNormal);
            cubeVertices[4] = VertexPositionNormal(bottomLeftFront, frontNormal);
            cubeVertices[3] = VertexPositionNormal(bottomRightFront, frontNormal);
            cubeVertices[5] = VertexPositionNormal(topRightFront, frontNormal);

            // Back face.
            cubeVertices[7] = VertexPositionNormal(topLeftBack, backNormal);
            cubeVertices[6] = VertexPositionNormal(topRightBack, backNormal);
            cubeVertices[8] = VertexPositionNormal(bottomLeftBack, backNormal);
            cubeVertices[10] = VertexPositionNormal(bottomLeftBack, backNormal);
            cubeVertices[9] = VertexPositionNormal(topRightBack, backNormal);
            cubeVertices[11] = VertexPositionNormal(bottomRightBack, backNormal);

            // Top face.
            cubeVertices[13] = VertexPositionNormal(topLeftFront, topNormal);
            cubeVertices[12] = VertexPositionNormal(topRightBack, topNormal);
            cubeVertices[14] = VertexPositionNormal(topLeftBack, topNormal);
            cubeVertices[16] = VertexPositionNormal(topLeftFront, topNormal);
            cubeVertices[15] = VertexPositionNormal(topRightFront, topNormal);
            cubeVertices[17] = VertexPositionNormal(topRightBack, topNormal);

            // Bottom face.
            cubeVertices[19] = VertexPositionNormal(bottomLeftFront, bottomNormal);
            cubeVertices[18] = VertexPositionNormal(bottomLeftBack, bottomNormal);
            cubeVertices[20] = VertexPositionNormal(bottomRightBack, bottomNormal);
            cubeVertices[22] = VertexPositionNormal(bottomLeftFront, bottomNormal);
            cubeVertices[21] = VertexPositionNormal(bottomRightBack, bottomNormal);
            cubeVertices[23] = VertexPositionNormal(bottomRightFront, bottomNormal);

            // Left face.
            cubeVertices[25] = VertexPositionNormal(topLeftFront, leftNormal);
            cubeVertices[24] = VertexPositionNormal(bottomLeftBack, leftNormal);
            cubeVertices[26] = VertexPositionNormal(bottomLeftFront, leftNormal);
            cubeVertices[28] = VertexPositionNormal(topLeftBack, leftNormal);
            cubeVertices[27] = VertexPositionNormal(bottomLeftBack, leftNormal);
            cubeVertices[29] = VertexPositionNormal(topLeftFront, leftNormal);

            // Right face.
            cubeVertices[31] = VertexPositionNormal(topRightFront, rightNormal);
            cubeVertices[30] = VertexPositionNormal(bottomRightFront, rightNormal);
            cubeVertices[32] = VertexPositionNormal(bottomRightBack, rightNormal);
            cubeVertices[34] = VertexPositionNormal(topRightBack, rightNormal);
            cubeVertices[33] = VertexPositionNormal(topRightFront, rightNormal);
            cubeVertices[35] = VertexPositionNormal(bottomRightBack, rightNormal);

            // Create the actual vertex buffer
            VertexBuffer=IRenderer::GetRendererInstance()->addVertexBuffer(VertexPositionNormal::VertexSizeInBytes * 36, STATIC, cubeVertices);

            VertexCount = 36;
            TriangleCount = 12;

	Shader = FW3ShadersFactory::GetShader(aShaderName, "main", "main");
	VertexDeclaration = FW3ShadersFactory::GetVertexFormat(eBox, Shader);

            VertexSizeInBytes = VertexPositionNormal::VertexSizeInBytes;

            
        }

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
        void Shape3D::CreateSphere(float radius, unsigned int slices, unsigned int stacks, const char* aShaderName)
        {
            
            float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float stackStep = D3DXFROMWINE_PI / stacks;
            unsigned int vertexCount = slices * (stacks - 1) + 2;
            unsigned int triangleCount = slices * (stacks - 1) * 2;
            unsigned int indexCount = triangleCount * 3;

            VertexPositionNormal* sphereVertices=new VertexPositionNormal[vertexCount];

            unsigned int currentVertex = 0;
            sphereVertices[currentVertex++] = VertexPositionNormal(D3DXFROMWINEVECTOR3(0, -radius, 0), D3DXFROMWINEVECTOR3(0, -1, 0));
            float stackAngle = D3DXFROMWINE_PI - stackStep;
            for (unsigned int i = 0; i < stacks - 1; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
                    //NOTE: y and z were switched from normal spherical coordinates because the sphere is "oriented" along the Y axis as opposed to the Z axis
                    float x = (float)(radius * sinf(stackAngle) * cosf(sliceAngle));
                    float y = (float)(radius * cosf(stackAngle));
                    float z = (float)(radius * sinf(stackAngle) * sinf(sliceAngle));

                    D3DXFROMWINEVECTOR3 position = D3DXFROMWINEVECTOR3(x, y, z);

			D3DXFROMWINEVECTOR3 normal = D3DXFROMWINEVECTOR3(x, y, z);

			D3DXFROMWINEVec3Normalize(&normal, &normal);

                   sphereVertices[currentVertex++] = VertexPositionNormal(position, normal);


                    sphereVertices[currentVertex++] = VertexPositionNormal(position, normal);

                    sliceAngle += sliceStep;
                }
                stackAngle -= stackStep;
            }
            sphereVertices[currentVertex++] = VertexPositionNormal(D3DXFROMWINEVECTOR3(0, radius, 0), D3DXFROMWINEVECTOR3(0, 1, 0));

            // Create the actual vertex buffer
		LOG_FNLN;
		LOG_PRINT("VertexSize=%x\n", VertexPositionNormal::VertexSizeInBytes);
		LOG_PRINT("vertexCount=%x\n", vertexCount);
		LOG_PRINT("sphereVertices=%x\n", sphereVertices);
	    VertexBuffer=IRenderer::GetRendererInstance()->addVertexBuffer(VertexPositionNormal::VertexSizeInBytes * vertexCount, STATIC, sphereVertices);
		delete[] sphereVertices;

            IndexBuffer = CreateIndexBuffer(vertexCount, indexCount, slices);

            VertexCount = vertexCount;
            TriangleCount = triangleCount;

	Shader = FW3ShadersFactory::GetShader(aShaderName, "main", "main");
	VertexDeclaration = FW3ShadersFactory::GetVertexFormat(eSphere, Shader);

            VertexSizeInBytes = VertexPositionNormal::VertexSizeInBytes;

            
        }

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
        void Shape3D::CreateCylinder(float bottomRadius, float topRadius, float length, unsigned int slices, unsigned int stacks, const char* aShaderName)
        {
		
            // if both the top and bottom have a radius of zero, just return 0, because invalid
            if (bottomRadius <= 0 && topRadius <= 0)
            {
                
            }
	    
            float sliceStep = 2.0f*D3DXFROMWINE_PI / slices;
            float heightStep = length / stacks;
            float radiusStep = (topRadius - bottomRadius) / stacks;
            float currentHeight = -length / 2;
            unsigned int vertexCount = (stacks + 1) * slices + 2;   //cone = stacks * slices + 1
            unsigned int triangleCount = (stacks + 1) * slices * 2; //cone = stacks * slices * 2 + slices
            unsigned int indexCount = triangleCount * 3;
            float currentRadius = bottomRadius;

            VertexPositionNormal* vertices=new VertexPositionNormal[vertexCount];

            // Start at the bottom of the cylinder
            unsigned int currentVertex = 0;
            vertices[currentVertex++] = VertexPositionNormal(D3DXFROMWINEVECTOR3(0, currentHeight, 0), D3DXFROMWINEVECTOR3(0, -1, 0));
            for (unsigned int i = 0; i <= stacks; i++)
            {
                float sliceAngle = 0;
                for (unsigned int j = 0; j < slices; j++)
                {
                    float x = currentRadius * (float)cosf(sliceAngle);
                    float y = currentHeight;
                    float z = currentRadius * (float)sinf(sliceAngle);

                    D3DXFROMWINEVECTOR3 position = D3DXFROMWINEVECTOR3(x, y, z);
			D3DXFROMWINEVECTOR3 normal = D3DXFROMWINEVECTOR3(x, y, z);

			D3DXFROMWINEVec3Normalize(&normal, &normal);

                    vertices[currentVertex++] = VertexPositionNormal(position, normal);

                    sliceAngle += sliceStep;
                }
                currentHeight += heightStep;
                currentRadius += radiusStep;
            }
            vertices[currentVertex++] = VertexPositionNormal(D3DXFROMWINEVECTOR3(0, length / 2, 0), D3DXFROMWINEVECTOR3(0, 1, 0));

            // Create the actual vertex buffer object
            VertexBuffer=IRenderer::GetRendererInstance()->addVertexBuffer(VertexPositionNormal::VertexSizeInBytes * vertexCount, STATIC, vertices);
		delete[] vertices;

            IndexBuffer = CreateIndexBuffer(vertexCount, indexCount, slices);

            VertexCount = vertexCount;
            TriangleCount = triangleCount;

	Shader = FW3ShadersFactory::GetShader(aShaderName, "main", "main");
	VertexDeclaration = FW3ShadersFactory::GetVertexFormat(eCylinder, Shader);

            VertexSizeInBytes = VertexPositionNormal::VertexSizeInBytes;

            
        }

        /// <summary>
        /// Creates an <see cref="IndexBuffer"/> for spherical shapes like Spheres, Cylinders, and Cones.
        /// </summary>
        /// <param name="device">The <see cref="GraphicsDevice"/> that is associated with the created cylinder.</param>
        /// <param name="vertexCount">The total number of vertices making up the </param>
        /// <param name="indexCount">The total number of indices making up the </param>
        /// <param name="slices">The number of slices about the Y axis.</param>
        /// <returns>The index buffer containing the index data for the </returns>
        IndexBufferID Shape3D::CreateIndexBuffer(unsigned int vertexCount, unsigned int indexCount, unsigned int slices)
        {
            unsigned int* indices=new unsigned int[indexCount];
            unsigned int currentIndex = 0;

            // Bottom circle/cone of shape
            for (unsigned int i = 1; i <= slices; i++)
            {
                indices[currentIndex++] = 0;
                indices[currentIndex++] = i;
                if (i - 1 == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
            }

            // Middle sides of shape
            for (unsigned int i = 1; i < vertexCount - (unsigned int)slices - 1; i++)
            {
                indices[currentIndex++] = i;
                indices[currentIndex++] = i + (unsigned int)slices;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i + (unsigned int)slices - 1;

                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
                indices[currentIndex++] = i;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
            }

            // Top circle/cone of shape
            for (unsigned int i = (unsigned int)vertexCount - (unsigned int)slices - 1; i < vertexCount - 1; i++)
            {
                indices[currentIndex++] = (unsigned int)vertexCount - 1;
                if ((i - 1) % slices == 0)
                    indices[currentIndex++] = i + (unsigned int)slices - 1;
                else
                    indices[currentIndex++] = i - 1;
                indices[currentIndex++] = i;
            }

            // Create the actual index buffer
		LOG_FNLN;
		LOG_PRINT("VertexSize=%x\n", sizeof(unsigned int));
		LOG_PRINT("indexCount=%x\n", indexCount);
		LOG_PRINT("indices=%x\n", indices);
            	IndexBufferID indexBuffer=IRenderer::GetRendererInstance()->addIndexBuffer(sizeof(unsigned int), indexCount, STATIC, indices);
		delete[] indices;

            return indexBuffer;
        }


std::map<std::string, ShaderID> FW3ShadersFactory::m_FW3ShadersFactory_Shadercache;
std::map<tShape, VertexFormatID> FW3ShadersFactory::m_FW3ShadersFactory_VertexFormatcache;

VertexFormatID FW3ShadersFactory::find_from_FW3ShadersFactory_VertexFormatcache(tShape aShape)
{
	std::map<tShape, VertexFormatID>::iterator it;
	it = m_FW3ShadersFactory_VertexFormatcache.find(tShape(aShape));
	if (it != m_FW3ShadersFactory_VertexFormatcache.end())
		return it->second;
	return -1;
}
void FW3ShadersFactory::insert_to_FW3ShadersFactory_VertexFormatcache(tShape aShape, VertexFormatID id)
{
	std::map<tShape, VertexFormatID>::iterator it;
	it = m_FW3ShadersFactory_VertexFormatcache.find(tShape(aShape));
	if (it == m_FW3ShadersFactory_VertexFormatcache.end())
		m_FW3ShadersFactory_VertexFormatcache.insert ( std::pair<tShape,VertexFormatID>(tShape(aShape), id) );
}
VertexFormatID FW3ShadersFactory::GetVertexFormat(tShape aShape, const ShaderID shd)
{
	VertexFormatID newvf;
	std::map<tShape, VertexFormatID>::iterator it;
	it = m_FW3ShadersFactory_VertexFormatcache.find(tShape(aShape));
	if (it != m_FW3ShadersFactory_VertexFormatcache.end())
		newvf=it->second;
	else
	{
		switch(aShape)
		{
			case eTexturedBox:
			{
				FormatDesc format[] =
				{
					0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
					0, TYPE_NORMAL,   FORMAT_FLOAT, 3,
					0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
				};
				newvf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
				break;
			}
			case eBox:
			case eSphere:
			case eCylinder:
			{
				FormatDesc format[] =
				{
					0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
					0, TYPE_NORMAL,   FORMAT_FLOAT, 3
				};
				newvf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
				break;
			}
		};
		m_FW3ShadersFactory_VertexFormatcache.insert ( std::pair<tShape,VertexFormatID>(tShape(aShape),newvf) );
	}
	return newvf;
	}

	ShaderID FW3ShadersFactory::find_from_FW3ShadersFactory_Shadercache(const char* aShaderName)
	{
		std::map<std::string, ShaderID>::iterator it;
		it = m_FW3ShadersFactory_Shadercache.find(std::string(aShaderName));
		if (it != m_FW3ShadersFactory_Shadercache.end())
			return it->second;
		return -1;
	}

	void FW3ShadersFactory::insert_to_FW3ShadersFactory_Shadercache(const char* aShaderName, ShaderID id)
	{
		std::map<std::string, ShaderID>::iterator it;
		it = m_FW3ShadersFactory_Shadercache.find(std::string(aShaderName));
		if (it == m_FW3ShadersFactory_Shadercache.end())
			m_FW3ShadersFactory_Shadercache.insert ( std::pair<std::string,ShaderID>(std::string(aShaderName), id) );
	}

	ShaderID FW3ShadersFactory::GetShader(const char* aShaderName, const char *vsName, const char *psName)
	{
		ShaderID newshd;
		std::map<std::string, ShaderID>::iterator it;
		it = m_FW3ShadersFactory_Shadercache.find(std::string(aShaderName));
		if (it != m_FW3ShadersFactory_Shadercache.end())
			newshd=it->second;
		else
		{
			const char* shdTxt=GetShaderTxt(aShaderName);
			newshd = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, vsName, psName);
			m_FW3ShadersFactory_Shadercache.insert ( std::pair<std::string,ShaderID>(std::string(aShaderName),newshd) );
		}
		return newshd;
	}

const char* FW3ShadersFactory::GetShaderTxt(const char* aShaderName)
{
#if defined(USE_GLES_SHADERS) && (defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
	return 0;
#elif defined(USE_GLES_SHADERS) && defined(_LINUX)
	return 0;
#elif defined(USE_HLSL_SHADERS)
if((stx_strcmp(aShaderName,"DiffuseLighting")==0)
|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
	return "\n";
	if(stx_strcmp(aShaderName,"AmbientLighting")==0)
		return "[Vertex shader]"
	"row_major float4x4 WorldViewProjection	: WORLDVIEWPROJECTION;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Pos : POSITION;\n"
		"float2 uv : TEXCOORD2;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
	"float4 Pos: POSITION;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
	"Out.Pos = mul(float4(IN.Pos,1), WorldViewProjection); // transform Position\n"
	"return Out;\n"
	"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
	"float4 Pos: POSITION;\n"
	"};\n"
	"float4 main(VS_OUTPUT IN) : COLOR\n"
	"{\n"
	"return float4(0.5, 0.075, 0.075, 1.0);\n"
	"}";
#else
	return 0;
#endif
	return 0;
}

