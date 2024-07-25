import subprocess
import sys
import os
import glob
import re


def vertify(compiler, input_dir, output_dir):
    if not os.path.exists(compiler):
        print('{0} not exist'.format(compiler))
        return False
    if not os.path.exists(input_dir):
        print('{0} not exist'.format(input_dir))
        return False
    if not os.path.exists(output_dir):
        print('{0} not exist'.format(output_dir))
        return False
    return True


def ReadHashCache(cache_file):
    with open(cache_file) as cf:
        content = cf.read()
    return content


def WriteHashCache(cache_file, content):
    with open(cache_file, 'w') as cf:
        print(content)
        cf.write(content)


def GetProtoHash(proto_dir):
    files = glob.glob('{0}/*'.format(proto_dir))
    cmd = ['cmake', '-E', 'md5sum']
    [cmd.append(f) for f in files]
    ret = subprocess.run(cmd, stdout=subprocess.PIPE)
    if not ret.returncode == 0:
        return ""
    else:
        return ret.stdout.decode('utf8')


def ProcessHash(s):
    hm = {}
    lines = s.split('\n')
    for line in lines:
        items = line.split(' ')
        if len(items) >= 2:
            hm[re.split('[\\\\/]', items[-1])[-1]] = items[0]
    return hm


def CompareCache(f, s):
    df = ProcessHash(f)
    ds = ProcessHash(s)
    if len(df) != len(ds):
        return False
    else:
        for k, v in df.items():
            if k not in ds.keys():
                return False
            else:
                vs = ds[k]
                # print(v, '-', vs, ' -- ', k)
                if vs != v:
                    return False
    return True


def NeedBuild(proto_dir, binary_dir):
    cache = '{0}/protocol.hashcache'.format(binary_dir)
    if(os.path.exists(cache)):
        saved_hash = ReadHashCache(cache)
        cur_hash = GetProtoHash(proto_dir)
        WriteHashCache(cache, cur_hash)
        return not CompareCache(saved_hash, cur_hash)
    else:
        cur_hash = GetProtoHash(proto_dir)
        WriteHashCache(cache, cur_hash)
        return True


def BuildProtocol(compiler, input_dir, output_dir):
    files = glob.glob('{0}/*'.format(input_dir))
    
    if compiler.find("boleidlc") != -1:
        cmd = [compiler, '--intf=bole1 --boleidl-path={0}'.format(input_dir), '--out={0}'.format(output_dir)]
        [cmd.append(i) for i in files if os.path.isfile(i)]
        cmd_str = ' '.join(cmd)
        ret = subprocess.run(cmd_str, shell=True)
    else:
        cmd = [compiler, '-I', input_dir, '--cpp_out={0}'.format(output_dir)]
        [cmd.append(i) for i in files if os.path.isfile(i)]
        ret = subprocess.run(cmd)

    if not ret.returncode == 0:
        print('compile proto files error')
        return False
    else:
        return True


def main(compiler, input_dir, output_dir, binary_dir):
    if not vertify(compiler, input_dir, output_dir):
        print("Argument Error.")
        return
    if NeedBuild(input_dir, binary_dir):
        print("Need compile protos")
        print('Start compiling protos ...')
        BuildProtocol(compiler, input_dir, output_dir)
        print('End compiled protos.')
    else:
        print("No need compile protos")


if __name__ == '__main__':
    # compiler input-dir output-dir
    binary_dir = os.getcwd()
    compiler = sys.argv[1]
    proto_src = sys.argv[2]
    output_dir = sys.argv[3]
    main(compiler, proto_src, output_dir, binary_dir)
