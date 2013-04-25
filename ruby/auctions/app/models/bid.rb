class BidRateValidator < ActiveModel::EachValidator

  def validate_each(record, attribute, value)
    # Validate this only when auction has been assigned.
    return if record.auction.nil?

    # Bid value should be at least 120% of previous best one.
    bidr = Bid.where(auction_id: record.auction.id).first
    old_bid = bidr ? bidr.value : 0
    unless value >= old_bid * 1.2
      record.errors[attribute] << "too low - expected at least #{old_bid * 1.2}, got #{value}."
    end
  end

end

class Bid < ActiveRecord::Base
  belongs_to :user
  belongs_to :auction
  attr_accessible :value, :auction_id

  before_save do |bid|
    # Truncate bid to two digits after radix point.
    bid.value = (bid.value * 100.0).ceil / 100.0
  end

  validates :user_id,
    presence: true

  validates :auction_id,
    presence: true

  validate :auction_activity
  
  validates :value,
    presence: true,
    bid_rate: true

  default_scope order: 'bids.value DESC'

  private

  def auction_activity
    return if auction.nil?
    errors.add(:base, 'Auction must be active.') unless auction.active?
  end

end
