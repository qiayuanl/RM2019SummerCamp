import random
import time

random.seed(time.time())

# outf=open("assessment.txt","w+")

namelist=["廖洽源","张哲慜","王冠","陈文潇","青雨晴","郑雨盺","朱彦臻"]

wordlist=[  "非常认真","积极主动","工作负责","能主动承担起自己分内的责任",
            "能完全胜任本职工作","十分努力","乐于助人","与组员相处融洽",
            "善于合作","积极肯干","责任心强","细心","协同组内工作","有创新能力",
            "为人诚信","待人谦虚","能为大家考虑","工作热情高","工作能力强",
            "团队意识强","经验丰富","帮助小组避免了很多错误", "善于提出建议",
            "协助别的方向完成任务","精益求精","细致入微","能够积极调动气氛","维持工作氛围"]

random.shuffle(namelist)

for name in namelist:
    outstr=""
    wordbuf=wordlist.copy()
    outstr+=(name+"\t")
    for i in range(random.randint(2,5)):
        outstr+=( wordbuf.pop( random.randint( 0 , len(wordbuf)-1 ) ) + "，" )
    outstr+=( wordbuf.pop( random.randint( 0 , len(wordbuf)-1 ) ) + "." )
    print(outstr)
    # print(outstr,file=outf)
