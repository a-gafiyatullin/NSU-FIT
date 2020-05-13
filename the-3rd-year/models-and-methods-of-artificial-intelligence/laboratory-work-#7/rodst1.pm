module rodst1;

var $n : tuple of atom := atom[];

var $ch : tuple of atom := atom[Ivan, Ekaterina];

var $win1;
var $wy : integer := 20;

rule enter
   forall $x: Human( name: $name )
=>
   $n := $n + atom[$name] ;
end;

rule enter1
=>
   var $m1,$m2 : integer;
   $m1 := Menu(400,50,"Choose man", $n, 1 ); 
   $ch[1]:= $n[$m1];
   OutText( $win1, 50, $wy, $ch[1]);
   $wy:=$wy+15;
   $m2 := Menu(400,50," Choose man", $n, 1 ); 
   $ch[2]:= $n[$m2];
   OutText( $win1, 50, $wy, $ch[2]);
   $wy:=$wy+30;
end;

rule enter2
exist $x: Human( name:$namex ),
      $y: Human( name:$namey )
  when ( $namex = $ch[1] ) & ( $namey = $ch[2] )
=> 
  new relat($x, $y);
end;

rule Stop
=>
  activate group();
end;

rule childrenParent
  forall $x: Human( parents: $par )
  when #$par != 0
=>
  for $i in $par loop
       edit   $i: Human( children:  $i.children + Human{$x} ); 
  end;
end;

rule ParentsChildren
  forall $x: Human(  partner: $partner, children: $children )
  when (#$children != 0) & ($partner != ?)
=>
  if #$partner.children = 0
  then edit   $partner: Human( children: $children );
  end;
  for $ii in $children loop
      if #$ii.parents != 2
      then edit   $ii: Human( parents: Human{$x, $partner }); 
      end; 
  end;
end;

/*----------------------------------------------------------------*/

rule Mother_Father
exist $rod : relat(who: $x, whom: $y),
      $x: Human( parents: $par )
      when ( #$par != 0 ) & ( $y in $par  )
=>
      if $y.gender = man 
      then   
           OutText( $win1, 50, $wy, ToString($y.name)+ " is father for "+ ToString($x.name) );
      else 
           OutText( $win1, 50, $wy,ToString($y.name)+ " is mother for "+ToString($x.name) );
       end; 
      
      $wy := $wy + 15;

      if $x.gender = man
      then  
           OutText( $win1, 50, $wy, ToString($x.name) + " is son for " + ToString($y.name) );
      else  
           OutText( $win1, 50, $wy, ToString($x.name)+" is daughter for " + ToString($y.name) );
      end; 
      $wy := $wy + 30; 
finish
      activate group();
end;

rule Grandmother_Grandfather
exist $rod : relat(who: $x, whom: $y),
      $x: Human( parents: $par ),
      $y: Human(children: $children )
      when ( #($par * $children) != 0 )
=>
 
      if $y.gender = man 
      then   
          OutText( $win1, 50, $wy, ToString($y.name) + " is grandfather for " + ToString($x.name) );
      else 
          OutText( $win1, 50, $wy, ToString($y.name) + " is grandmother for " + ToString($x.name) );
      end;
      
      $wy := $wy + 15;
 
      if $x.gender = man
      then  
          OutText( $win1, 50, $wy, ToString($x.name) + " is  grandson for " + ToString($y.name) );
      else  
          OutText( $win1, 50, $wy, ToString($x.name) + " is granddaugter for " + ToString($y.name) );
      end; 

      $wy := $wy + 30; 
finish
	activate group();
 end;

rule Brother_Sister
  exist $rod : relat(who: $x, whom: $y),
         $x: Human( parents: $par ),
         $y: Human( parents: $par )
         when (#$par != 0 )
=>

         if $x.gender = man 
         then   
             OutText( $win1, 50, $wy, ToString($x.name) + " is brother for " + ToString($y.name) );
         else 
             OutText( $win1, 50, $wy, ToString($x.name) + " is sister for " + ToString($y.name) );
         end; 

         $wy := $wy + 15;  

         if $y.gender = man
         then  
             OutText( $win1, 50, $wy, ToString($y.name) + " is brother for " + ToString($x.name) );
         else  
             OutText( $win1, 50, $wy, ToString($y.name) + " is sister for " + ToString($x.name) );
         end; 
      $wy := $wy + 30;   
finish
	activate group();
end;

rule Partner
  exist $rod : relat(who: $x, whom: $y ),
        $x: Human( name:$name, partner: $y )
=>
         if $x.gender = man 
         then   
             OutText( $win1, 50, $wy, ToString($x.name) + " is husband for " + ToString($y.name) );
         else 
             OutText( $win1, 50, $wy, ToString($x.name) + " is wife for " + ToString($y.name) );
         end; 

         $wy := $wy + 15;   

         if $y.gender = man
         then  
             OutText( $win1, 50, $wy, ToString($y.name) + " is husband for " + ToString($x.name) );
         else  
             OutText( $win1, 50, $wy, ToString($y.name) + " is wife for " + ToString($x.name) );
         end; 

      $wy := $wy + 30;   
finish
	activate group();
end;

rule Cousins
  exist $rod : relat(who: $x, whom: $y),
         $x: Human( parents: $par1 ),
         $y: Human( parents: $par2 )
         when ( #($par1)!=0 & #($par2) != 0 & $par1!=$par2)
=>
        for $ii in $par1 loop
          for $ij in $par2 loop
            if #$ii.parents!=0 & #$ij.parents!=0 & $ii.parents=$ij.parents
            then 
              if $x.gender = man 
              then   
                 OutText( $win1, 50, $wy, ToString($x.name) + " is cousin for " + ToString($y.name) );
              else 
                 OutText( $win1, 50, $wy, ToString($x.name) + " is cousin for " + ToString($y.name) );
              end; 

              $wy := $wy + 15;   

              if $y.gender = man
              then  
                  OutText( $win1, 50, $wy, ToString($y.name) + " is cousin for " + ToString($x.name) );
              else  
                  OutText( $win1, 50, $wy, ToString($y.name) + " is cousin for " + ToString($x.name) );
              end; 
              $wy := $wy + 30;   
            end; //if 
          end; //for ij
        end; //for ii
finish
	activate group();
end;

rule MotherInLaw 
  exist $rod : relat(who: $x, whom: $y ),
        $x : Human(partner:$partnerx),    
        $y : Human(partner:$partnery)
        when($y.gender != $x.gender & (($y in $partnerx.parents & $y.gender = woman) | ($x in $partnery.parents & $x.gender = woman)))
=>
	if $y in $partnerx.parents
	then
	    OutText( $win1, 50, $wy, ToString($y.name) + " is mother-in-law for " + ToString($x.name) );
	end;  

	$wy := $wy + 15; 

	if $x in $partnery.parents
	then
      OutText( $win1, 50, $wy, ToString($x.name) + " is mother-in-law for " + ToString($y.name) );
	end; 

      $wy := $wy + 30;   
finish
	activate group();
end;
   
rule Norelat
  exist $rod : relat(who: $x, whom: $y)
=>
  OutText( $win1, 50, $wy, ToString($x.name) + " not in relationship with " + ToString($y.name) );
end;

 var $rules : group := group( childrenParent, ParentsChildren, enter, enter1, enter2, 
                               Mother_Father, Grandmother_Grandfather,  Brother_Sister,
                              Partner, Cousins, MotherInLaw, 
			      Norelat, Stop );

begin
  new

    @Ivan: Human (  name: Ivan, gender: man ),

    @Ekaterina: Human (  name: Ekaterina,  partner: @Ivan ),

    @Alexander: Human (  name: Alexander ),

    @Anna: Human (  name: Anna, gender: woman, partner: @Alexander ),

    @Lena: Human (  name: Lena, gender: woman ),

    @Misha: Human (  name: Misha, gender: man ),

    @Vasya: Human (  name: Vasya, gender: man ),

    @Valentine: Human (  name: Valentine,  
  	                parents: (Human { @Ekaterina , @Ivan  }) ),

    @Andrew: Human (  name: Andrew, gender: man, 
                      partner: @Valentine, 
        	      children: (Human{ @Lena , @Misha, @Vasya }) ,
        	      parents: Human{ @Alexander, @Anna } ),

    @Olya: Human (  name: Olya, gender: woman ), 
 
    @Dima: Human (  name: Dima, gender: man ), 

    @Sergei: Human (  name: Sergei,
	                parents: Human { @Ekaterina, @Ivan } ),

    @Inna: Human (  name: Inna, gender: woman, 
   	                  partner: @Sergei,
	                  children: Human{ @Olya, @Dima } ), 
	
    @Natasha: Human (  name: Natasha, gender: woman ),
 
    @Anya: Human (  name: Anya, gender: woman ), 
 
    @Lyudmila: Human (  name: Lyudmila, gender: woman,
	              children: Human { @Natasha, @Anya } ,
	              parents: Human{ @Alexander , @Anna} ),

    @Vladimir: Human (  name: Vladimir,
	              partner: @Lyudmila ), 
 
    @Tamara: Human (  name: Tamara  ), 

    @Alex: Human (  name: Alex, gender: man ), 

    @Ira: Human (  name: Ira, gender: woman ),

    @Peter: Human (  name: Peter, gender: man, 
 	                partner: @Tamara, 
	                children: Human { @Alex, @Ira },
	                parents: Human{ @Alexander , @Anna } );

   $win1 := MakeWindow("Question-answer system 'relatives'", 0,0,1010,640);  
   TextColor($win1,4);
   call $rules;
   if (Ask("End of session", "Close windows?"))
   then  CloseWindow($win1);
   end; 
WriteNet();

end.