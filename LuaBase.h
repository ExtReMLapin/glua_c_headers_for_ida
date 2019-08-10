#ifndef GARRYSMOD_LUA_LUABASE_H
# define GARRYSMOD_LUA_LUABASE_H

struct Vector {
  float x;
  float y;
  float z;
};

struct QAngle {
  float x;
  float y;
  float z;
};

enum PushSpecial {
  SPECIAL_GLOB, // Global table
  SPECIAL_ENV, // Environment table
  SPECIAL_REG, // Registry table
};


struct lua_State;


typedef int( * CFunc)(lua_State * L);

struct ILuaBase {

  // You shouldn't need to use this struct
  // Instead, use the UserType functions
  struct UserData {
    void * data;
    unsigned char type;
  };

  //void            (*vector)(double*, double*, struct s_obj*, int);

  // Returns the amount of values on the stack
  int( * Top)(void);

  // Pushes a copy of the value at iStackPos to the top of the stack
  void( * Push)(int);

  // Pops iAmt values from the top of the stack
  void( * Pop)(int);

  // Pushes table[key] on to the stack
  // table = value at iStackPos
  // key   = value at top of the stack
  void( * GetTable)(int);

  // Pushes table[key] on to the stack
  // table = value at iStackPos
  // key   = strName
  void( * GetField)(int,
    const char * );

  // Sets table[key] to the value at the top of the stack
  // table = value at iStackPos
  // key   = strName
  // Pops the value from the stack
  void( * SetField)(int,
    const char * );

  // Creates a new table and pushes it to the top of the stack
  void( * CreateTable)();

  // Sets table[key] to the value at the top of the stack
  // table = value at iStackPos
  // key   = value 2nd to the top of the stack
  // Pops the key and the value from the stack
  void( * SetTable)(int);

  // Sets the metatable for the value at iStackPos to the value at the top of the stack
  // Pops the value off of the top of the stack
  void( * SetMetaTable)(int);

  // Pushes the metatable of the value at iStackPos on to the top of the stack
  // Upon failure, returns false and does not push anything
  bool( * GetMetaTable)(int);

  // Calls a function
  // To use it: Push the function on to the stack followed by each argument
  // Pops the function and arguments from the stack, leaves iResults values on the stack
  // If this function errors, any local C values will not have their destructors called!
  void( * Call)(int, int);

  // Similar to Call
  // See: lua_pcall( lua_State*, int, int, int )
  int( * PCall)(int, int, int);

  // Returns true if the values at iA and iB are equal
  int( * Equal)(int, int);

  // Returns true if the value at iA and iB are equal
  // Does not invoke metamethods
  int( * RawEqual)(int, int);

  // Moves the value at the top of the stack in to iStackPos
  // Any elements above iStackPos are shifted upwards
  void( * Insert)(int);

  // Removes the value at iStackPos from the stack
  // Any elements above iStackPos are shifted downwards
  void( * Remove)(int);

  // Allows you to iterate tables similar to pairs(...)
  // See: lua_next( lua_State*, int );
  int( * Next)(int);

  // Deprecated: Use the UserType functions instead of this
  void( ** NewUserdata)(unsigned int);

  // Throws an error and ceases execution of the function
  // If this function is called, any local C values will not have their destructors called!
  void( * ThrowError)(const char * );

  // Checks that the type of the value at iStackPos is iType
  // Throws and error and ceases execution of the function otherwise
  // If this function errors, any local C values will not have their destructors called!
  void( * CheckType)(int, int);

  // Throws a pretty error message about the given argument
  // If this function is called, any local C values will not have their destructors called!
  void( * ArgError)(int,
    const char * );

  // Pushes table[key] on to the stack
  // table = value at iStackPos
  // key   = value at top of the stack
  // Does not invoke metamethods
  void( * RawGet)(int);

  // Sets table[key] to the value at the top of the stack
  // table = value at iStackPos
  // key   = value 2nd to the top of the stack
  // Pops the key and the value from the stack
  // Does not invoke metamethods
  void( * RawSet)(int);

  // Returns the string at iStackPos. iOutLen is set to the length of the string if it is not NULL
  // If the value at iStackPos is a number, it will be converted in to a string
  // Returns NULL upon failure
  const char( ** GetString)(int, unsigned int * );

  // Returns the number at iStackPos
  // Returns 0 upon failure
  double( * GetNumber)(int);

  // Returns the boolean at iStackPos
  // Returns false upon failure
  bool( * GetBool)(int);

  // Returns the C-Function at iStackPos
  // returns NULL upon failure
  CFunc( * GetCFunction)(int);

  // Deprecated: You should probably be using the UserType functions instead of this
  void( ** GetUserdata)(int);

  // Pushes a nil value on to the stack
  void( * PushNil)();

  // Pushes the given string on to the stack
  // If iLen is 0, strlen will be used to determine the string's length
  void( * PushString)(const char * , unsigned int);

  // Pushes the given double on to the stack
  void( * PushNumber)(double);

  // Pushes the given bobolean on to the stack
  void( * PushBool)(bool);

  // Pushes the given C-Function on to the stack
  void( * PushCFunction)(CFunc);

  // Pushes the given C-Function on to the stack with upvalues
  // See: GetUpvalueIndex()
  void( * PushCClosure)(CFunc, int);

  // Pushes the given pointer on to the stack as light-userdata
  void( * PushUserdata)(void * );

  // Allows for values to be stored by reference for later use
  // Make sure you call ReferenceFree when you are done with a reference
  int( * ReferenceCreate)();
  void( * ReferenceFree)(int);
  void( * ReferencePush)(int);

  // Push a special value onto the top of the stack (see SPECIAL_* enums)
  void( * PushSpecial)(PushSpecial);

  // Returns true if the value at iStackPos is of type iType
  // See: Types.h
  bool( * IsType)(int, int);

  // Returns the type of the value at iStackPos
  // See: Types.h
  int( * GetType)(int);

  // Returns the name associated with the given type ID
  // See: Types.h
  // Note: GetTypeName does not work with user-created types
  const char( ** GetTypeName)(int);

  // Deprecated: Use CreateMetaTable
  void( * CreateMetaTableType)(const char * , int);

  // Like Get* but throws errors and returns if they're not of the expected type
  // If these functions error, any local C values will not have their destructors called!
  const char( ** CheckString)(int);
  double( * CheckNumber)(int);

  // Returns the length of the object at iStackPos
  // Works for: strings, tables, userdata
  int( * ObjLen)(int);

  // Returns the angle at iStackPos
  const QAngle( ** GetAngle)(int);

  // Returns the vector at iStackPos
  const Vector( ** GetVector)(int);

  // Pushes the given angle to the top of the stack
  void( * PushAngle)(const QAngle * );

  // Pushes the given vector to the top of the stack
  void( * PushVector)(const Vector * );

  // Sets the lua_State to be used by the ILuaBase implementation
  // You don't need to use this if you use the LUA_FUNCTION macro
  void( * SetState)(lua_State * L);

  // Pushes the metatable associated with the given type name
  // Returns the type ID to use for this type
  // If the type doesn't currently exist, it will be created
  int( * CreateMetaTable)(const char * );

  // Pushes the metatable associated with the given type
  bool( * PushMetaTable)(int);

  // Creates a new UserData of type iType that references the given data
  void( * PushUserType)(void * , int);

  // Sets the data pointer of the UserType at iStackPos
  // You can use this to invalidate a UserType by passing NULL
  void( * SetUserType)(int, void * );

  // Returns the data of the UserType at iStackPos if it is of the given type

};

// For use with ILuaBase::PushSpecial

struct lua_State {
  unsigned char _ignore_this_common_lua_header_[48 + 22];
  ILuaBase * luabase;
};


#endif
