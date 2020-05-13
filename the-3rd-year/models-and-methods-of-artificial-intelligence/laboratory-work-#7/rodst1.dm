definition rodst;

type Gender = atom( man, woman );

class Human;

class Human
  name:		atom;
  gender:		Gender;
  partner: Human;
  children: set of Human := Human{};      
  parents:  set of Human := Human{};
constraints
    p_gender: gender != partner.gender;
    p_partner: partner.partner = $;
end;

relation relat ( who: Human; whom: Human)
sym;
end;

end.