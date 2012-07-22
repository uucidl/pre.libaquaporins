Given /^a segment with tracks:$/ do |track_definition|
  step 'I have created a piece'
  step 'I have created a segment'
  track_definition.hashes.each do |hash|
    track_name = hash['track_name']
    signature  = hash['signature']
    step "I have created a track '#{track_name}' with signature '#{signature}'"
  end
end

Given /^the track '(\w+)' contains:$/ do |track_name, row_definition|
  n = row_definition.rows.length

  step "I created a write range [0, #{n}[ on track '#{track_name}'"

  row_definition.rows.each do |r|
    if r[0] != 'nil' then
      step "I write #{r[0]} to the range"
    end
    step "I advance to the next row"
  end

  step "I commit the write range"
end

Then /^the checksum of track '(\w+)' should be (\d+)$/ do |track_name, checksum|
  step "I created a read range on track '#{track_name}'"
  step "The sum of the range should be #{checksum}"
end
