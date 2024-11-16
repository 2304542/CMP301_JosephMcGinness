#include "Terrain.h"

/*void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(mInfo.HeightmapWidth * mInfo.HeightmapHeight);
	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightMapFilename.c_str(),
		std::ios_base::binary);
	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());
		// Done with file.
		inFile.close();
	}
	// Copy the array data into a float array and scale it.
	mHeightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
	for (UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
	{
		mHeightmap[i] = (in[i] / 255.0f) * mInfo.HeightScale;
	}
} */
