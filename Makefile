zip::
	zip -r --symlinks ../backup/zips/at-gromacs_`date +'%Y-%m-%d-%H%M'`.zip \
	    --exclude="*.zip" \
	    --exclude="*.vscode*" \
	    --exclude="*.git*" \
	    *

clean::
	$(MAKE) $@ -C src
