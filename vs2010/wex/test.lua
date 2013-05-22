require "class"

Person = class()

function Person:__init(name, age)
    self.name = name
    self.age = age
end

function Person:fart()
    print(self.name .. " farts.")
end

person = Person("Bob", 41)

person:fart()

print(type(person))

for k,v in pairs(person) do
    print(k)
end
