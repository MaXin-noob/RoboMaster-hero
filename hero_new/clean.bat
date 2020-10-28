@cd ./Build
@echo开始删除............
@del *.crf /s
@del *.o /s
@del *.d /s
@del *.htm /s
@del *.dep/s
@del *.sct/s
@del *.axf/s
@del *.hex/s
@del *.map/s
@del *.lnp/s
@cd ./../MDK-ARM
@del hero.uvguix.*/s

@cd ./../Src
@del *.orig/s
@echo 删除完成
@exit
