#pragma once

namespace Utopia
{
	struct Profiling
	{
		static void startSection(const char* section);
		static void endSection(const char* section);
	};
}
