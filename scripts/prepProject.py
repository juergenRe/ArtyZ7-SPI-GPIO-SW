from pathlib import Path, PurePosixPath
import sys
import json
import xmltodict


class ConverterFactory(object):
    @staticmethod
    def getConverter(cfg: dict):
        cnvType = cfg.get('type', '')
        if cnvType.lower() == 'json':
            return JsonConverter(cfg)
        elif cnvType.lower() == 'xml':
            return XmlConverter(cfg)
        else:
            print('Wrong Type. No Converter available')
            return None


class Converter(object):
    def __init__(self, config: dict):
        self._fileName = None
        self._config = config
        self._fileContents = {}

    def _readContents(self, fpth: Path) -> dict:
        raise ValueError

    def _writeContents(self, fpth: Path, contents: dict):
        raise ValueError

    def _transform(self, inData: dict):
        raise ValueError

    def convertFile(self, fpth: Path):
        if len(fpth.name) > 0:
            self._fileName = fpth

        inData = self._readContents(fpth)
        self._fileContents = self._transform(inData)
        self._writeContents(fpth, self._fileContents)


class JsonConverter(Converter):
    def __init__(self, config: dict):
        super().__init__(config)

    def _transform(self, inData: dict):
        for entry in self._config['entries']:
            p = inData.get(entry, '')
            if str(self._config['projBase']) in p:
                idx = len(self._config['projBase'])
                basep = Path(self._config['baseDirName'])
                relPath = Path(str(basep), p[idx:])
                inData[entry] = relPath

        return inData

    def _readContents(self, fpth: Path) -> dict:
        with fpth.open() as f:
            s = json.load(f)
        return s

    def _writeContents(self, fpth: Path, contents: dict):
        s = json.dumps(contents)
        fpth.write_text(s)


class XmlConverter(Converter):
    def __init__(self, config: dict):
        super().__init__(config)

    def _transform(self, inData: dict):
        trxDict = inData.get(self._config.get('baseEntry', 'xxxxx'), inData)
        for entry in self._config['entries']:
            p = trxDict.get(entry, '')
            if str(self._config['projBase']) in p:
                idx = len(str(self._config['projBase']))
                basep = PurePosixPath(*self._config['baseDirName'])
                relPath = PurePosixPath(str(basep), p[idx+1:])
                trxDict[entry] = relPath

        return inData

    def _readContents(self, fpth: Path) -> dict:
        s = xmltodict.parse(fpth.read_text())
        return s

    def _writeContents(self, fpth: Path, contents: dict):
        s = xmltodict.unparse(contents)
        fpth.write_text(s)


class PrjConverter(XmlConverter):
    def __init__(self, config: dict):
        super().__init__(config)



#==================================================================

if __name__ == '__main__':
    try:
        configFile = Path('prepProject.json')
        with configFile.open() as f:
            config = json.load(f)

        projectExtensions = config['projectExtensions']
    except:
        print('Problem reading configuration file. Aborting')
        sys.exit()

    actPath = Path.cwd()
    if actPath.name != 'scripts':
        print('This script need to be started in the "scripts" folder of a project. Aborting')
        sys.exit()

    projBase = actPath.parent
    pp = list(projBase.parts)
    pp[0] = pp[0][:-1]
    projBaseLx = PurePosixPath(*pp)

    # get all folders within base
    projFoldersRaw = [f for f in projBase.iterdir() if f.is_dir()]
    projFolders = [f for f in projFoldersRaw if f.name[0] != '.' and f.name not in config.get('excludeFolders', '')]

    # get all project files
    pf = []
    for d in projFolders:
        l = [f for f in d.glob('*') if f.suffix in projectExtensions.keys()]
        pf.extend(l)

    # now read files and correct them
    for prjf in pf:
        cfg = projectExtensions.get(prjf.suffix, {})
        cfg['projBase'] = projBaseLx
        if len(cfg) == 0:
            print(f'No configuration found for type {prjf.suffix}')
        else:
            cnv = ConverterFactory().getConverter(cfg)
            cnv.convertFile(prjf)
    print('Done')