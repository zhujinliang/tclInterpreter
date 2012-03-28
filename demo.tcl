puts "demo"

proc multiply {x y} 
{
    $x * $y
}

set b 5
set c 6
puts "The multiply of $b $c is: [multiply $b $c]"
puts "$b << 2 is: [$b << 2]"
puts "~$b is: [~ $b]"
puts "!$b is: [! $b]"

set a 1
while {$a < 5} 
{  
    w8 $a $a
    r8 $a
    set a [$a + 1]; # that means a = a + 1
}
puts "a = $a"
#exit
set a (0:2) 0xa
puts $a
puts [a (0:2)]
set b [a (0:2)]
puts "b= $b"

set str "the end"
puts $str
