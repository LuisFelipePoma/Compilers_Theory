; ModuleID = 'LaPC2'
source_filename = "LaPC2"

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

declare i32 @puts(ptr)

define void @_main_() {
entry:
  %c = alloca double, align 8
  %b = alloca double, align 8
  %a = alloca double, align 8
  store double 1.000000e+00, ptr %a, align 8
  store double 1.100000e+01, ptr %b, align 8
  store double 1.200000e+01, ptr %c, align 8
  %callSystem = call i32 @puts(ptr @1)
  ret void
}

define void @hola(double %x, double %y, double %z) {
entry:
  %callSystem = call i32 @puts(ptr @0)
  ret void
}
