set filename=%1

set rawname=%filename:.png=.raw%

set redname=%rawname:high_mid_low=high%
set bluename=%rawname:high_mid_low=mid%
set greenname=%rawname:high_mid_low=low%

stream -map r -storage-type char %filename% %redname%
stream -map g -storage-type char %filename% %bluename%
stream -map b -storage-type char %filename% %greenname%