; ModuleID = 'LaPC2'
source_filename = "LaPC2"

define void @_anon_() {
entry:
  %b = alloca double, align 8
  %a = alloca double, align 8
  store double 1.200000e+01, ptr %a, align 8
  store double 2.200000e+01, ptr %b, align 8
  ret void
}
