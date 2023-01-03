definition computer_problem;

type problem_type = string("monitor", "storage device", "program", "performance");

type complaint = string("no picture", "screen tearing", "low screen resolution",
			"Linux", "processor without integrated graphics",

			"GPU", "external drive not determined by OS", "hard drive",
			"flash card", "not formatted external drive",

			"Java-program", "Python-program", "not modified environment variables",
			"GPU not determined by OS",

			"computer assembled by yourself", "4-slots RAM", "not setted setting of the RAM on BIOS");

type solution = string("Check monitor sync settings.", "The proprietary video card driver cannot load. Add a free driver to the blacklist.",
		       "No video signal. Check the video output of your PC for a video card connected to it.",

			"SATA cable has departed. Check the connection of the hard drive to the motherboard.",
			"USB port is not connected to the motherboard. Try other ports or check the connection of this to the motherboard.",
			"Corrupted FS. Format the media in FAT32 FS.",

			"Java virtual machine or environment variables for it are not installed.",
			"Python interpreter or environment variables for it are not installed.",
			"The video card driver is not installed.",

			"RAM operates in single-channel mode.",
			"The maximum possible memory frequency in the BIOS is not set.");

relation Problem(comp : complaint; ptype : problem_type);

class ProblemSolving
	problem_solution : solution;
	problems : set of complaint:= complaint{};
end;

end.
