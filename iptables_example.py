#!/usr/bin/python3.5
import iptc

uname = 'u4372'
dst = '{}-dst'.format(uname)
src = '{}-src'.format(uname)


table = iptc.Table(iptc.Table.FILTER)
if not table.is_chain(uname):
  iptc.easy.add_chain('filter', uname)

chain = iptc.Chain(table, uname)
print(chain.name)

# create chain
#iptc.easy.add_chain('filter', uname)
#create rule to jump in chain

rule = iptc.Rule()
#rule.protocol = "tcp"
rule.target = rule.create_target(uname)
m = rule.create_match("set")
m.match_set = [dst, 'dst']
chain = iptc.Chain(iptc.Table(iptc.Table.FILTER), "FORWARD")
chain.insert_rule(rule)
