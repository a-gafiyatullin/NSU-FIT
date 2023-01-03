module computer_problems;

var $win1;
var $wy : integer := 20;

var $problems : set of Problem := Problem{};
var $solutions : set of ProblemSolving := ProblemSolving{};
var $all_problems : tuple of complaint := complaint[];
var $my_problems : set of complaint := complaint{};
var $expert_system_type : integer;

// заполяняем массив с проблемами
rule inputProblems
	forall $x: Problem()
=>
	$problems := $problems + Problem{$x};
end;

// заполяняем массив с решениями проблем
rule inputSolutions
	forall $x: ProblemSolving()
=>
	$solutions := $solutions + ProblemSolving{$x};
end;

// правило получения возможных проблем для указанного решения
rule getProblemsForSolution
=>
	var $type_num : integer;
	var $j: integer;
	var $problem_types : tuple of problem_type := problem_type["monitor", "storage device", "program", "performance"];
	var $possible_solutions_set : set of ProblemSolving := ProblemSolving{};
	var $possible_solutions_tuple : tuple of solution := solution[];

	// выбираем тип проблемы
	$type_num := Menu(400, 50, "You have problem with:", $problem_types, 1);

	// находим все проблесы с указанным типом
	for $e in $problems loop
		if $e.ptype = $problem_types[$type_num]
		then
			$all_problems := $all_problems + complaint[$e.comp];
		end;
	end;

	// находим все решения с такими проблеми
	for $x in $all_problems loop
		for $y in $solutions loop
			if $x in $y.problems then
				$possible_solutions_set := $possible_solutions_set + ProblemSolving{$y};
			end;
		end;
	end;

	// конвертируем множество в массив для меню
	for $x in $possible_solutions_set loop
		$possible_solutions_tuple := $possible_solutions_tuple + solution[$x.problem_solution];
	end;
	// получаем проблему
	$type_num := Menu(400, 50, "For problems from this class, the following solutions exist:", $possible_solutions_tuple, 1);
	OutText($win1, 50, $wy, "The following symptoms indicate that this solution may solve your problem:");
	// выводим признаки указанной проблемы
	$j := 1;
	for $x in $possible_solutions_set loop
		if $j = $type_num then
			for $y in $x.problems loop
				$wy := $wy + 15;
				OutText($win1, 75, $wy, "* " + $y+ ";");
			end;
		end;
		$j := $j + 1;
	end;
end;

// выбираем тип проблемы
rule getProblemType
=>
	var $type_num : integer;
	var $problem_types : tuple of problem_type := problem_type["monitor", "storage device", "program", "performance"];

	$type_num:= Menu(400, 50, "You have problem with:", $problem_types, 1);

	// находим все проблесы с указанным типом
	for $e in $problems loop
		if $e.ptype = $problem_types[$type_num]
		then
			$all_problems := $all_problems + complaint[$e.comp];
		end;
	end;
end;

// процедура, которая ограничивает выбор в меню проблем
procedure getNewAllProblemsList()
begin
	var $possible_problems : set of complaint := complaint{};

	// проходимся по всем решениям и выбираем проблемы тех решений, которые могут подходить под уже выбранные проблемы
	for $x in $solutions loop
		if $my_problems <= $x.problems then
			for $e in $x.problems loop
				if $e in $my_problems then
				else
					$possible_problems := $possible_problems + complaint{$e};
				end;
			end;
		end;
	end;

	$all_problems := complaint[];
	for $x in $possible_problems loop
		$all_problems := $all_problems + complaint[$x];
	end;		
end;

// выбор проблем, пока есть из чего выбирать
rule chooseProblems
=>
	var $i : integer;

	while #$all_problems != 0 loop
		$i := Menu(400, 50, "You have:", $all_problems, 1);
		$my_problems := $my_problems + complaint{$all_problems[$i]};

		getNewAllProblemsList();	
	end;
end;

// получение решения по проблемам
rule findSolution
	exist $x: ProblemSolving(problem_solution: $sol, problems : $prob)
	when forall $y in $my_problems: $y in $prob
=>
	OutText($win1, 50, $wy, $sol);
	$wy := $wy + 30;
	// объяснение решения
	OutText($win1, 50, $wy, "This decision was received because you have:");
	for $x in $prob loop
		$wy := $wy + 15;
		OutText($win1, 75, $wy, "* " + $x + ";");
	end;
end;

rule Stop 
=>
	activate group();
end;

var $rules_search :  group := group(inputProblems, inputSolutions, getProblemType, chooseProblems, findSolution, Stop);
var $rules_reverse_search :  group := group(inputProblems, inputSolutions, getProblemsForSolution, Stop);

begin
	new Problem(comp : "no picture", ptype : "monitor");
	new Problem(comp : "screen tearing", ptype : "monitor");
	new Problem(comp : "low screen resolution", ptype : "monitor");
	new Problem(comp : "Linux", ptype : "monitor");
	new Problem(comp : "processor without integrated graphics", ptype : "monitor");
	new Problem(comp : "GPU", ptype : "monitor");

	new Problem(comp : "external drive not determined by OS", ptype : "storage device");
	new Problem(comp : "hard drive", ptype : "storage device");
	new Problem(comp : "flash card", ptype : "storage device");
	new Problem(comp : "not formatted external drive", ptype : "storage device");

	new Problem(comp : "Java-program", ptype : "program");
	new Problem(comp : "Python-program", ptype : "program");
	new Problem(comp : "not modified environment variables", ptype : "program");
	new Problem(comp : "GPU not determined by OS", ptype : "program");

	new Problem(comp : "computer assembled by yourself", ptype : "performance");
	new Problem(comp : "4-slots RAM", ptype : "performance");
	new Problem(comp : "not setted setting of the RAM on BIOS", ptype : "performance");

new
	@screen_solution1 : ProblemSolving(problem_solution: "Check monitor sync settings.",
		problems : {"screen tearing"}),
	@screen_solution2 : ProblemSolving(problem_solution: "The proprietary video card driver cannot load. Add a free driver to the blacklist.",
		problems : {"low screen resolution", "Linux", "GPU"}),
	@screen_solution3 : ProblemSolving(problem_solution: "No video signal. Check the video output of your PC for a video card connected to it.",
		problems : {"no picture", "processor without integrated graphics", "GPU"}),
	@storage_solution1 : ProblemSolving(problem_solution: "SATA cable has departed. Check the connection of the hard drive to the motherboard.",
		problems : {"external drive not determined by OS", "hard drive"}),
	@storage_solution2 : ProblemSolving(problem_solution: "USB port is not connected to the motherboard. Try other ports or check the connection of this to the motherboard.",
		problems : {"external drive not determined by OS", "flash card"}),
	@storage_solution3 : ProblemSolving(problem_solution: "Corrupted FS. Format the media in FAT32 FS.",
		problems : {"not formatted external drive"}),
	@program_solution1 : ProblemSolving(problem_solution: "Java virtual machine or environment variables for it are not installed.",
		problems : {"Java-program", "not modified environment variables"}),
	@program_solution2 : ProblemSolving(problem_solution: "Python interpreter or environment variables for it are not installed.",
		problems : {"Python-program", "not modified environment variables"}),
	@program_solution3 : ProblemSolving(problem_solution: "The video card driver is not installed.",
		problems : {"GPU not determined by OS"}),
	@performance_solution1 : ProblemSolving(problem_solution: "RAM operates in single-channel mode.",
		problems : {"computer assembled by yourself", "4-slots RAM"}),
	@performance_solution2 : ProblemSolving(problem_solution: "The maximum possible memory frequency in the BIOS is not set.",
		problems : {"computer assembled by yourself", "not setted setting of the RAM on BIOS"});
	
	$win1 := MakeWindow("Results: ", 0, 0, 1010, 640);  
	TextColor($win1,1);
	$expert_system_type := Menu(400, 50, "You want get", ["solution by problems", "problems by solution"], 1);
	if $expert_system_type = 1 then
		call $rules_search;
	else
		call $rules_reverse_search;
	end;
	if (Ask("That's all!", "Close?"))
		then CloseWindow($win1);
	end;
end.