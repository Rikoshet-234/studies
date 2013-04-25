class Tagging < ActiveRecord::Base
  belongs_to :tag
  belongs_to :auction
  # attr_accessible :title, :body
end
